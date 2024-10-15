// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

#include "videodec2_avc.h"

namespace Core::Loader {
class SymbolsResolver;
}
namespace Libraries::Vdec2 {

class VdecDecoder;

using OrbisVideodec2Decoder = VdecDecoder*;
typedef void* OrbisVideodec2ComputeQueue;

struct OrbisVideodec2DecoderConfigInfo {
    u64 thisSize;
    u32 resourceType;
    u32 codecType;
    u32 profile;
    u32 maxLevel;
    s32 maxFrameWidth;
    s32 maxFrameHeight;
    s32 maxDpbFrameCount;
    u32 decodePipelineDepth;
    OrbisVideodec2ComputeQueue computeQueue;
    u64 cpuAffinityMask;
    s32 cpuThreadPriority;
    bool optimizeProgressiveVideo;
    bool checkMemoryType;
    u8 reserved0;
    u8 reserved1;
    void* extraConfigInfo;
};

struct OrbisVideodec2DecoderMemoryInfo {
    u64 thisSize;
    u64 cpuMemorySize;
    void* pCpuMemory;
    u64 gpuMemorySize;
    void* pGpuMemory;
    u64 cpuGpuMemorySize;
    void* pCpuGpuMemory;
    u64 maxFrameBufferSize;
    u32 frameBufferAlignment;
    u32 reserved0;
};

struct OrbisVideodec2InputData {
    u64 thisSize;
    void* pAuData;
    u64 auSize;
    u64 ptsData;
    u64 dtsData;
    u64 attachedData;
};

struct OrbisVideodec2OutputInfo {
    u64 thisSize;
    bool isValid;
    bool isErrorFrame;
    u8 pictureCount;
    u32 codecType;
    u32 frameWidth;
    u32 framePitch;
    u32 frameHeight;
    void* pFrameBuffer;
    u64 frameBufferSize;
};

struct OrbisVideodec2FrameBuffer {
    u64 thisSize;
    void* pFrameBuffer;
    u64 frameBufferSize;
    bool isAccepted;
};

struct OrbisVideodec2ComputeMemoryInfo {
    u64 thisSize;
    u64 cpuGpuMemorySize;
    void* pCpuGpuMemory;
};

struct OrbisVideodec2ComputeConfigInfo {
    u64 thisSize;
    u16 computePipeId;
    u16 computeQueueId;
    bool checkMemoryType;
    u8 reserved0;
    u16 reserved1;
};

s32 PS4_SYSV_ABI
sceVideodec2QueryComputeMemoryInfo(OrbisVideodec2ComputeMemoryInfo* pComputeMemInfoOut);

s32 PS4_SYSV_ABI
sceVideodec2AllocateComputeQueue(const OrbisVideodec2ComputeConfigInfo* pComputeCfgInfoIn,
                                 const OrbisVideodec2ComputeMemoryInfo* pComputeMemInfoIn,
                                 OrbisVideodec2ComputeQueue* pComputeQueueOut);

s32 PS4_SYSV_ABI sceVideodec2ReleaseComputeQueue(OrbisVideodec2ComputeQueue computeQueueIn);

s32 PS4_SYSV_ABI
sceVideodec2QueryDecoderMemoryInfo(const OrbisVideodec2DecoderConfigInfo* pDecoderConfigInfoIn,
                                   OrbisVideodec2DecoderMemoryInfo* pDecoderMemoryInfoOut);

s32 PS4_SYSV_ABI
sceVideodec2CreateDecoder(const OrbisVideodec2DecoderConfigInfo* pDecoderConfigInfoIn,
                          const OrbisVideodec2DecoderMemoryInfo* pDecoderMemoryInfoIn,
                          OrbisVideodec2Decoder* pDecoderInstanceOut);

s32 PS4_SYSV_ABI sceVideodec2DeleteDecoder(OrbisVideodec2Decoder decoder);

s32 PS4_SYSV_ABI sceVideodec2Decode(OrbisVideodec2Decoder decoder,
                                    const OrbisVideodec2InputData* pInputDataInOut,
                                    OrbisVideodec2FrameBuffer* pFrameBufferInOut,
                                    OrbisVideodec2OutputInfo* pOutputInfoOut);

s32 PS4_SYSV_ABI sceVideodec2Flush(OrbisVideodec2Decoder decoder,
                                   OrbisVideodec2FrameBuffer* pFrameBufferInOut,
                                   OrbisVideodec2OutputInfo* pOutputInfoOut);

s32 PS4_SYSV_ABI sceVideodec2Reset(OrbisVideodec2Decoder decoder);

s32 PS4_SYSV_ABI sceVideodec2GetPictureInfo(const OrbisVideodec2OutputInfo* pOutputInfoIn,
                                            void* p1stPictureInfoOut, void* p2ndPictureInfoOut);

void RegisterlibSceVdec2(Core::Loader::SymbolsResolver* sym);
} // namespace Libraries::Vdec2