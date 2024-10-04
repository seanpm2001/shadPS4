// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "videodec2.h"
#include "videodec2_impl.h"

#include "common/assert.h"
#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/libs.h"

namespace Libraries::Vdec2 {

static constexpr u64 kMinimumMemorySize = 32_MB; ///> Fake minimum memory size for querying

int PS4_SYSV_ABI sceVideodec2QueryComputeMemoryInfo(OrbisVideodec2ComputeMemoryInfo* pMemInfoOut) {
    LOG_INFO(Lib_Vdec2, "called");

    if (pMemInfoOut->thisSize != sizeof(OrbisVideodec2ComputeMemoryInfo)) {
        return 0x811d0101;
    }

    pMemInfoOut->pCpuGpuMemory = nullptr;
    pMemInfoOut->cpuGpuMemorySize = kMinimumMemorySize;

    return ORBIS_OK;
}

s32 PS4_SYSV_ABI
sceVideodec2AllocateComputeQueue(const OrbisVideodec2ComputeConfigInfo* pComputeCfgInfoIn,
                                 const OrbisVideodec2ComputeMemoryInfo* pComputeMemInfoIn,
                                 OrbisVideodec2ComputeQueue* pComputeQueueOut) {
    LOG_INFO(Lib_Vdec2, "called");
    return ORBIS_OK;
}

s32 PS4_SYSV_ABI sceVideodec2ReleaseComputeQueue(OrbisVideodec2ComputeQueue computeQueueIn) {
    LOG_INFO(Lib_Vdec2, "called");
    return ORBIS_OK;
}

int PS4_SYSV_ABI
sceVideodec2QueryDecoderMemoryInfo(const OrbisVideodec2DecoderConfigInfo* pCfgInfoIn,
                                   OrbisVideodec2DecoderMemoryInfo* pMemInfoOut) {
    LOG_INFO(Lib_Vdec2, "called");

    if ((pCfgInfoIn->thisSize == 0x48) && (pMemInfoOut->thisSize == 0x48)) {
        pMemInfoOut->pCpuMemory = nullptr;
        pMemInfoOut->pGpuMemory = nullptr;
        pMemInfoOut->pCpuGpuMemory = nullptr;

        pMemInfoOut->cpuGpuMemorySize = kMinimumMemorySize;
        pMemInfoOut->cpuMemorySize = kMinimumMemorySize;
        pMemInfoOut->gpuMemorySize = kMinimumMemorySize;

        pMemInfoOut->maxFrameBufferSize = kMinimumMemorySize;
        pMemInfoOut->frameBufferAlignment = kMinimumMemorySize;
    } else {
        return 0x811d0101;
    }

    return ORBIS_OK;
}

s32 PS4_SYSV_ABI
sceVideodec2CreateDecoder(const OrbisVideodec2DecoderConfigInfo* pDecoderConfigInfoIn,
                          const OrbisVideodec2DecoderMemoryInfo* pDecoderMemoryInfoIn,
                          OrbisVideodec2Decoder* pDecoderInstanceOut) {
    LOG_INFO(Lib_Vdec2, "called");

    *pDecoderInstanceOut = new VdecDecoder(*pDecoderConfigInfoIn, *pDecoderMemoryInfoIn);
    return ORBIS_OK;
}

s32 PS4_SYSV_ABI sceVideodec2DeleteDecoder(OrbisVideodec2Decoder decoder) {
    LOG_INFO(Lib_Vdec2, "called");
    delete decoder;
    return ORBIS_OK;
}

s32 PS4_SYSV_ABI sceVideodec2Decode(OrbisVideodec2Decoder decoder,
                                    const OrbisVideodec2InputData* pInputDataInOut,
                                    OrbisVideodec2FrameBuffer* pFrameBufferInOut,
                                    OrbisVideodec2OutputInfo* pOutputInfoOut) {
    LOG_TRACE(Lib_Vdec2, "called");

    if (decoder == nullptr) {
        return 0x811D0103; // SCE_VIDEODEC2_ERROR_DECODER_INSTANCE;
    }

    return decoder->Decode(*pInputDataInOut, *pFrameBufferInOut, *pOutputInfoOut);
}

s32 PS4_SYSV_ABI sceVideodec2Flush(OrbisVideodec2Decoder decoder,
                                   OrbisVideodec2FrameBuffer* pFrameBufferInOut,
                                   OrbisVideodec2OutputInfo* pOutputInfoOut) {
    LOG_INFO(Lib_Vdec2, "called");

    if (decoder == nullptr) {
        return 0x811D0103;
    }

    return decoder->Flush(*pFrameBufferInOut, *pOutputInfoOut);
}

s32 PS4_SYSV_ABI sceVideodec2Reset(OrbisVideodec2Decoder decoder) {
    LOG_INFO(Lib_Vdec2, "called");

    if (decoder == nullptr) {
        return 0x811D0103;
    }

    return decoder->Reset();
}

s32 PS4_SYSV_ABI sceVideodec2GetPictureInfo(const OrbisVideodec2OutputInfo* pOutputInfoIn,
                                            void* p1stPictureInfoOut, void* p2ndPictureInfoOut) {
    LOG_TRACE(Lib_Vdec2, "called");
    if (pOutputInfoIn->pictureCount == 0 || gPictureInfos.empty()) {
        return 0;
    }

    if (p1stPictureInfoOut) {
        OrbisVideodec2AvcPictureInfo* picInfo =
            static_cast<OrbisVideodec2AvcPictureInfo*>(p1stPictureInfoOut);
        *picInfo = gPictureInfos.back();
    }

    if (pOutputInfoIn->pictureCount > 1) {
        UNREACHABLE();
    }

    return ORBIS_OK;
}

void RegisterlibSceVdec2(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("RnDibcGCPKw", "libSceVideodec2", 1, "libSceVideodec2", 1, 1,
                 sceVideodec2QueryComputeMemoryInfo);
    LIB_FUNCTION("eD+X2SmxUt4", "libSceVideodec2", 1, "libSceVideodec2", 1, 1,
                 sceVideodec2AllocateComputeQueue);
    LIB_FUNCTION("UvtA3FAiF4Y", "libSceVideodec2", 1, "libSceVideodec2", 1, 1,
                 sceVideodec2ReleaseComputeQueue);

    LIB_FUNCTION("qqMCwlULR+E", "libSceVideodec2", 1, "libSceVideodec2", 1, 1,
                 sceVideodec2QueryDecoderMemoryInfo);
    LIB_FUNCTION("CNNRoRYd8XI", "libSceVideodec2", 1, "libSceVideodec2", 1, 1,
                 sceVideodec2CreateDecoder);
    LIB_FUNCTION("jwImxXRGSKA", "libSceVideodec2", 1, "libSceVideodec2", 1, 1,
                 sceVideodec2DeleteDecoder);
    LIB_FUNCTION("852F5+q6+iM", "libSceVideodec2", 1, "libSceVideodec2", 1, 1, sceVideodec2Decode);
    LIB_FUNCTION("l1hXwscLuCY", "libSceVideodec2", 1, "libSceVideodec2", 1, 1, sceVideodec2Flush);
    LIB_FUNCTION("wJXikG6QFN8", "libSceVideodec2", 1, "libSceVideodec2", 1, 1, sceVideodec2Reset);
    LIB_FUNCTION("NtXRa3dRzU0", "libSceVideodec2", 1, "libSceVideodec2", 1, 1,
                 sceVideodec2GetPictureInfo);
}

} // namespace Libraries::Vdec2