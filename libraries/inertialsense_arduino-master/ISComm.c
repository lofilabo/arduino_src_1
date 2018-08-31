/*
MIT LICENSE

Copyright 2014 Inertial Sense, LLC - http://inertialsense.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ISComm.h"

const unsigned int g_validBaudRates[IS_BAUDRATE_COUNT] = { IS_BAUDRATE_3000000, IS_BAUDRATE_921600, IS_BAUDRATE_460800, IS_BAUDRATE_230400, IS_BAUDRATE_115200, IS_BAUDRATE_57600, IS_BAUDRATE_38400, IS_BAUDRATE_19200 };

// Replace special character with encoded equivalent and add to buffer
static uint8_t* encodeByteAddToBuffer(unsigned val, uint8_t* ptrDest)
{
	switch (val)
	{
	case PSC_ASCII_START_BYTE:
	case PSC_ASCII_END_BYTE:
	case PSC_START_BYTE:
	case PSC_END_BYTE:
	case PSC_RESERVED_KEY:
	case UBLOX_START_BYTE1:
	case RTCM3_START_BYTE:
		*ptrDest++ = PSC_RESERVED_KEY;
		*ptrDest++ = ~val;
		break;
	default:
		*ptrDest++ = val;
		break;
	}

	return ptrDest;
}

static int dataIdShouldSwap(uint32_t dataId)
{
	switch (dataId)
	{
	case DID_GPS1_VERSION:
	case DID_GPS2_VERSION:
		return 0;
	}
	return 1;
}

static void swapPacket(packet_t* pkt)
{
	if (pkt->hdr.flags & CM_PKT_FLAGS_RAW_DATA_NO_SWAP)
	{
		if ((pkt->hdr.pid == PID_DATA || pkt->hdr.pid == PID_SET_DATA) && pkt->body.size >= sizeof(p_data_hdr_t))
		{
			// swap the data header only
			flipEndianess32(pkt->body.ptr, sizeof(p_data_hdr_t));
		}
	}
	else if (pkt->body.size < sizeof(p_data_hdr_t) || (pkt->hdr.pid != PID_DATA && pkt->hdr.pid != PID_SET_DATA))
	{
		// swap entire packet, not a data packet
		flipEndianess32(pkt->body.ptr, pkt->body.size);
	}
	else
	{
		// swap header
		flipEndianess32(pkt->body.ptr, sizeof(p_data_hdr_t));

		// get header
		p_data_hdr_t* dataHdr = (p_data_hdr_t*)pkt->body.ptr;

		// if dev_info_t, swap only the uint32 fields, this data structure is handled special as it contains char[] arrays and uint32_t in the same struct
		if (dataHdr->id == DID_DEV_INFO && pkt->body.size == sizeof(p_data_hdr_t) + sizeof(dev_info_t))
		{
			// swap only the pieces that need swapping
			dev_info_t* devInfo = (dev_info_t*)(pkt->body.ptr + sizeof(p_data_hdr_t));
			devInfo->buildNumber = SWAP32(devInfo->buildNumber);
			devInfo->repoRevision = SWAP32(devInfo->repoRevision);
			devInfo->serialNumber = SWAP32(devInfo->serialNumber);
		}
		else if (dataIdShouldSwap(dataHdr->id))
		{
			// swap entire packet body
			flipEndianess32(pkt->body.ptr + sizeof(p_data_hdr_t), pkt->body.size - sizeof(p_data_hdr_t));

			// flip doubles
			uint16_t* offsets;
			uint16_t offsetsLength;
			uint8_t* dataBuf = pkt->body.ptr + sizeof(p_data_hdr_t);

			// flip doubles back if needed
			if ((offsets = getDoubleOffsets(dataHdr->id, &offsetsLength)))
			{
				flipDoubles(dataBuf, dataHdr->size, dataHdr->offset, offsets, offsetsLength);
			}

			// flip strings back if needed
			if ((offsets = getStringOffsetsLengths(dataHdr->id, &offsetsLength)))
			{
				flipStrings(dataBuf, dataHdr->size, dataHdr->offset, offsets, offsetsLength);
			}
		}
	}
}

void is_comm_init(is_comm_instance_t* instance)
{
	instance->errors = 0;
	instance->counter = 0;
	instance->bufferPtr = 0;
	instance->bufferPtrEnd = 0;
	instance->hasStartByte = 0;
}

uint32_t is_comm_parse(is_comm_instance_t* instance, uint8_t byte)
{
	if (instance == 0 || instance->buffer == 0 || instance->bufferSize < 16)
	{
		return DID_NULL;
	}
	else if (byte == PSC_START_BYTE)
	{
		// set flag and reset pos to beginning
		instance->hasStartByte = 1;
		instance->bufferPtr = instance->buffer;
		instance->bufferPtrEnd = instance->buffer + instance->bufferSize;
	}

	if (instance->hasStartByte)
	{
		if (instance->bufferPtr == instance->bufferPtrEnd)
		{
			// end of buffer, reset state
			instance->hasStartByte = 0;
		}
		else
		{
			// append byte
			*instance->bufferPtr++ = byte;

			// if we appended the end byte, decode the packet
			if (byte == PSC_END_BYTE)
			{
				// create a packet to decode into
				uint32_t dataId = 0;
				packet_t packet;
				packet.body.ptr = instance->buffer;
				packet.body.size = instance->bufferSize;

				// decode the packet in place on top of the receive buffer to save memory
				if (is_decode_binary_packet(&packet, instance->buffer, (int)(instance->bufferPtr - instance->buffer)) == 0 && packet.hdr.pid == PID_DATA)
				{
					p_data_hdr_t* dataHdr = (p_data_hdr_t*)packet.body.ptr;
					dataId = dataHdr->id;
					uint32_t dataSize = dataHdr->size;
					uint32_t dataOffset = dataHdr->offset;
					if (dataId > DID_NULL && dataId < DID_COUNT && dataSize > 0 && dataOffset + dataSize <= MAX_DATASET_SIZE)
					{
						// copy over the actual data structure to the start of the buffer
						memmove(instance->buffer + dataOffset, packet.body.ptr + sizeof(p_data_hdr_t), dataSize);
					}
					else
					{
						// corrupt data
						dataId = DID_NULL;
					}
				}
				else
				{
					// corrupt data
					instance->errors++;
				}

				// reset position to beginning
				instance->hasStartByte = 0;

				// return data id, this will be a valid data id if the packet size and offset look good, otherwise DID_NULL
				return dataId;
			}
		}
	}
	else if (byte == PSC_END_BYTE)
	{
		// reset position to beginning
		instance->hasStartByte = 0;
	}

	return DID_NULL;
}

int is_comm_get_data(is_comm_instance_t* instance, uint32_t dataId, uint32_t offset, uint32_t size, uint32_t periodMS)
{
	p_data_get_t request;

	request.id = dataId;
	request.offset = offset;
	request.size = size;
	request.bc_period_ms = periodMS;

	packet_hdr_t hdr;
	hdr.flags = 0;
	hdr.pid = PID_GET_DATA;
	hdr.counter = instance->counter++;

	return is_encode_binary_packet(&request, sizeof(request), &hdr, 0, instance->buffer, instance->bufferSize);
}

int is_comm_set_data(is_comm_instance_t* instance, uint32_t dataId, uint32_t offset, uint32_t size, void* data)
{
	int dataSize = size + sizeof(p_data_hdr_t);
	uint8_t* toSend = (uint8_t*)MALLOC(dataSize);
	memcpy(toSend + sizeof(p_data_hdr_t), data, size);
	p_data_hdr_t* dataHdr = (p_data_hdr_t*)toSend;
	dataHdr->id = dataId;
	dataHdr->size = size;
	dataHdr->offset = offset;

	packet_hdr_t hdr;
	hdr.flags = 0;
	hdr.pid = PID_SET_DATA;
	hdr.counter = instance->counter++;

	int result = is_encode_binary_packet(toSend, dataSize, &hdr, 0, instance->buffer, instance->bufferSize);
	FREE(toSend);
	return result;
}

int is_comm_stop_broadcasts(is_comm_instance_t* instance)
{
	packet_hdr_t hdr;
	hdr.flags = 0;
	hdr.pid = PID_STOP_ALL_BROADCASTS;
	hdr.counter = instance->counter++;

	return is_encode_binary_packet(0, 0, &hdr, 0, instance->buffer, instance->bufferSize);
}

void is_decode_binary_packet_footer(packet_ftr_t* ftr, uint8_t* ptrSrc, uint8_t** ptrSrcEnd, uint32_t* checksum)
{
	int state = 0;
	uint8_t* currentPtr = (*ptrSrcEnd) - 1;
	*(uint32_t*)ftr = 0;

	// we need a state machine to ensure we don't overrun ptrSrcEnd
	while (state != 7 && currentPtr > ptrSrc)
	{
		switch (state)
		{
		case 0: // packet end byte
			ftr->stopByte = *currentPtr--;
			state = 1;
			break;

		case 1: // packet checksum 1
			ftr->cksum1 = *currentPtr--;
			state = (3 - (*currentPtr == PSC_RESERVED_KEY));
			break;

		case 2: // packet checksum 1 is encoded
			ftr->cksum1 = ~ftr->cksum1;
			currentPtr--;
			state = 3;
			break;

		case 3: // packet checksum 2
			ftr->cksum2 = *currentPtr--;
			state = (5 - (*currentPtr == PSC_RESERVED_KEY));
			break;

		case 4: // packet checksum 2 is encoded
			ftr->cksum2 = ~ftr->cksum2;
			currentPtr--;
			state = 5;
			break;

		case 5: // packet checksum 3
			ftr->cksum3 = *currentPtr;
			state = (7 - (*(currentPtr - 1) == PSC_RESERVED_KEY));
			break;

		case 6: // packet checksum 3 is encoded
			ftr->cksum3 = ~ftr->cksum3;
			currentPtr--;
			state = 7;
			break;

		default:
			break;
		}
	}
	*ptrSrcEnd = currentPtr;
	*checksum = ((uint32_t)ftr->cksum1) | (0x0000FF00 & ((uint32_t)ftr->cksum2 << 8)) | (0x00FF0000 & ((uint32_t)ftr->cksum3 << 16));
}

int is_decode_binary_packet_byte(uint8_t** _ptrSrc, uint8_t** _ptrDest, uint32_t* checksum, uint32_t shift)
{
	uint8_t* ptrSrc = *_ptrSrc;

	// packet id byte
	uint32_t val = *ptrSrc++;
	switch (val)
	{
	case PSC_ASCII_START_BYTE:
	case PSC_ASCII_END_BYTE:
	case PSC_START_BYTE:
	case PSC_END_BYTE:
	case RTCM3_START_BYTE:
	case UBLOX_START_BYTE1:
		// corrupt data
		return -1;

	case PSC_RESERVED_KEY:
		// skip special byte
		val = (~(*ptrSrc++) & 0x000000FF);
		// fall through intentional

	default:
		*checksum ^= (val << shift);
		*((*_ptrDest)++) = val;
	}
	*_ptrSrc = ptrSrc;

	return 0;
}

int is_encode_binary_packet(void* srcBuffer, unsigned int srcBufferLength, packet_hdr_t* hdr, uint8_t additionalPktFlags, void* encodedPacket, int encodedPacketLength)
{
	// Ensure data size is small enough, assuming packet size could double after encoding.
	if (srcBufferLength > MAX_PKT_BODY_SIZE)
	{
		return -1;
	}

	// Update Packet Counter
	uint8_t* ptrSrc;
	uint8_t* ptrSrcEnd;
	uint8_t* ptrDest = (uint8_t*)encodedPacket;
	uint8_t* ptrDestEnd = ptrDest + encodedPacketLength;
	uint32_t shifter = 0;
	uint32_t checkSumValue = CHECKSUM_SEED;
	uint32_t val;

	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	// Packet header -------------------------------------------------------------------------------------------
	*ptrDest++ = PSC_START_BYTE;

	// PID
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = hdr->pid;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);
	checkSumValue ^= val;

	// Counter
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = hdr->counter;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);
	checkSumValue ^= (val << 8);

	// Flags
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = hdr->flags | additionalPktFlags | CPU_IS_LITTLE_ENDIAN | CM_PKT_FLAGS_CHECKSUM_24_BIT;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);
	checkSumValue ^= (val << 16);

	// Packet body ----------------------------------------------------------------------------------------------
	if (srcBufferLength > 0)
	{
		ptrSrc = (uint8_t*)srcBuffer;
		ptrSrcEnd = ptrSrc + srcBufferLength;

		// copy body bytes, doing encoding and checksum
		while (ptrSrc != ptrSrcEnd && ptrDest < ptrDestEnd)
		{
			val = *ptrSrc++;
			checkSumValue ^= (val << shifter);

			// increment shifter
			shifter += 8;

			// reset if shifter equals 24
			shifter *= (shifter != 24);

			ptrDest = encodeByteAddToBuffer(val, ptrDest);
		}
	}

	// footer ----------------------------------------------------------------------------------------------------

	// checksum byte 3
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = (uint8_t)((checkSumValue >> 16) & 0xFF);
	ptrDest = encodeByteAddToBuffer(val, ptrDest);

	// checksum byte 2
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = (uint8_t)(checkSumValue >> 8) & 0xFF;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);

	// checksum byte 1
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = (uint8_t)(checkSumValue & 0xFF);
	ptrDest = encodeByteAddToBuffer(val, ptrDest);

	// packet end byte
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	*ptrDest++ = PSC_END_BYTE;
    return (int)(ptrDest - (uint8_t*)encodedPacket);
}

int is_decode_binary_packet(packet_t* pkt, unsigned char* pbuf, int pbufSize)
{
	// before we even get in this method, we can be assured that pbuf starts with a packet start byte and ends with a packet end byte
	// all other data can potentially be garbage
	if (pbufSize < 8)
	{
		// corrupt data
		return -1;
	}

	// decode the body and calculate checksum
	uint8_t* ptrSrc = pbuf;
	uint8_t* ptrDest = (uint8_t*)&pkt->hdr;
	uint8_t* ptrSrcEnd = pbuf + pbufSize;
	packet_ftr_t ftr;
	uint32_t actualCheckSumValue;

	is_decode_binary_packet_footer(&ftr, ptrSrc, &ptrSrcEnd, &actualCheckSumValue);
	uint32_t shifter = 0;
	uint32_t checkSumValue = CHECKSUM_SEED;

	// start packet byte
	*ptrDest++ = *ptrSrc++;

	if
	(
		// packet id
		is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, 0) ||

		// packet counter
		is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, 8) ||

		// packet flags
		is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, 16)
	)
	{
		return -1;
	}

	// decode the body - start shift 0
	ptrDest = pkt->body.ptr;
	while (ptrSrc < ptrSrcEnd)
	{
		if (is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, shifter))
		{
			return -1;
		}

		shifter += 8;

		// reset if shifter equals 24
		shifter *= (shifter != 24);
	}

	if (actualCheckSumValue != checkSumValue)
	{
		// corrupt data
		return -1;
	}

	pkt->body.size = (uint32_t)(ptrDest - pkt->body.ptr);
	if (pkt->body.size > MAX_PKT_BODY_SIZE)
	{
		return -1;
	}

	// if the endianness of the packet doesn't match our CPU, we need to flip the data so it will be correct for our CPU architecture
	else if (pkt->body.size != 0 && (pkt->hdr.flags & CM_PKT_FLAGS_ENDIANNESS_MASK) != CPU_IS_LITTLE_ENDIAN)
	{
		swapPacket(pkt);
	}

	return 0;
}
