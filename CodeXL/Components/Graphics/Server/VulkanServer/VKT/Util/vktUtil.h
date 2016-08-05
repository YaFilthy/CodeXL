//==============================================================================
/// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file   vktUtil.h
/// \brief  Header file for VktUtil namespace.
///         This contains various utility functions/macros/includes used
///         throughout the Vulkan server.
//==============================================================================

#ifndef __VKT_UTIL_H__
#define __VKT_UTIL_H__

#if defined _LINUX
    #include <signal.h>
#endif

// Cross-platform
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdio.h>

#include <vulkan.h>
#include <vk_layer.h>

#include <AMDTBaseTools/Include/AMDTDefinitions.h>

// Disable some warnings set off by LunarG-provided code
#if AMDT_BUILD_TARGET == AMDT_WINDOWS_OS
    #pragma warning (push)
    #pragma warning (disable : 4005)
    #pragma warning (disable : 4996)
    #pragma warning (disable : 4100)
#elif AMDT_BUILD_TARGET == AMDT_LINUX_OS
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#else
    #error Unknown build target! No valid value for AMDT_BUILD_TARGET.
#endif

#include <vk_loader_platform.h>
#include <vk_layer_table.h>
#include <vk_layer_extension_utils.h>
#include <vk_icd.h>

// pop the warning suppression pragmas
#if AMDT_BUILD_TARGET == AMDT_WINDOWS_OS
    #pragma warning (pop)
    #pragma warning (disable : 4505)
#elif AMDT_BUILD_TARGET == AMDT_LINUX_OS
    #pragma GCC diagnostic pop
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include "../../../Common/CommonTypes.h"
#include "../../../Common/misc.h"
#include "../vktEnumerations.h"

class VktWrappedCmdBuf;
class VktWrappedQueue;

//-----------------------------------------------------------------------------
/// Basic device information
//-----------------------------------------------------------------------------
struct DeviceInfo
{
    VkPhysicalDevice physicalDevice; ///< The physical device
    VkDevice         device;         ///< The device
};

//-----------------------------------------------------------------------------
/// Basic queue information
//-----------------------------------------------------------------------------
struct QueueInfo
{
    VkPhysicalDevice physicalDevice; ///< The queue's physical device
    VkDevice         device;         ///< The queue's device
    VktWrappedQueue* pWrappedQueue;  ///< The queue's wrapper
};

//-----------------------------------------------------------------------------
/// A pair of calibration timestamps used to align GPU events alongside the CPU timeline.
//-----------------------------------------------------------------------------
struct CalibrationTimestampPair
{
    UINT64        mBeforeExecutionCPUTimestamp; ///< A CPU timestamp
    UINT64        mBeforeExecutionGPUTimestamp; ///< A GPU timestamp
    GPS_TIMESTAMP cpuFrequency;                 ///< The clock frequency for the CPU that executed API calls
    UINT64        mQueueFrequency;              ///< The clock frequency for the Queue that work was submitted through
};

//-----------------------------------------------------------------------------
/// The client should fill one of these in, to uniquely identify each measurement.
//-----------------------------------------------------------------------------
struct ProfilerMeasurementId
{
    UINT64            sampleId;        ///< The sample ID for this measurement
    UINT              funcId;          ///< The function currently being profiled
    UINT              frame;           ///< The frame being profiled
    VktWrappedCmdBuf* pWrappedCmdBuf;  ///< Pointers to parent command buffer
    UINT64            fillId;          ///< An ID used to track how many times a cmdBuf was filled in
    VktWrappedQueue*  pWrappedQueue;   ///< The queue we gathered results from
};

/// The maximum size allowed for string-ified entry point arguments
#define ARGUMENTS_BUFFER_SIZE 8192

/// Zero out memory
#define VKT_ZERO_MEMORY(_ptr_, _size_)  memset(_ptr_, 0, _size_)

/// Safe object deletion
#define VKT_SAFE_DELETE(__expr__) if (__expr__) delete __expr__; __expr__ = nullptr;

/// Safe object array deletion
#define VKT_SAFE_DELETE_ARRAY(__expr__) if (__expr__) delete [] __expr__; __expr__ = nullptr;

/// Print out enum members as string
#define PRINTENUMCASE(inEnum, outString) case inEnum: outString = #inEnum; break;

/// Used to print out enums
typedef const char* (WriteEnum_Hook)(UINT flags);

//-----------------------------------------------------------------------------
/// Util and debug functions for VKT.
//-----------------------------------------------------------------------------
namespace VktUtil
{
void ConstructMeasurementInfo(FuncId inFuncId, UINT64 sampleId, VktWrappedCmdBuf* pWrappedCmdBuf, UINT frameNum, UINT64 fillId, ProfilerMeasurementId& measurementId);
void DecomposeFlags(UINT flags, gtASCIIString& ioFlagsString, WriteEnum_Hook inWriteHook, UINT inMinFlag, UINT inMaxFlag);

const char* WritePipelineCacheHeaderVersionEnumAsString(int enumVal);
const char* WriteResultCodeEnumAsString(int enumVal);
const char* WriteStructureTypeEnumAsString(int enumVal);
const char* WriteSystemAllocationScopeEnumAsString(int enumVal);
const char* WriteInternalAllocationTypeEnumAsString(int enumVal);
const char* WriteFormatEnumAsString(int enumVal);
const char* WriteImageTypeEnumAsString(int enumVal);
const char* WriteImageTilingEnumAsString(int enumVal);
const char* WritePhysicalDeviceTypeEnumAsString(int enumVal);
const char* WriteQueryTypeEnumAsString(int enumVal);
const char* WriteSharingModeEnumAsString(int enumVal);
const char* WriteImageLayoutEnumAsString(int enumVal);
const char* WriteImageViewTypeEnumAsString(int enumVal);
const char* WriteComponentSwizzleEnumAsString(int enumVal);
const char* WriteVertexInputRateEnumAsString(int enumVal);
const char* WritePrimitiveTopologyEnumAsString(int enumVal);
const char* WritePolygonModeEnumAsString(int enumVal);
const char* WriteFrontFaceEnumAsString(int enumVal);
const char* WriteCompareOpEnumAsString(int enumVal);
const char* WriteStencilOpEnumAsString(int enumVal);
const char* WriteLogicOpEnumAsString(int enumVal);
const char* WriteBlendFactorEnumAsString(int enumVal);
const char* WriteBlendOpEnumAsString(int enumVal);
const char* WriteDynamicStateEnumAsString(int enumVal);
const char* WriteFilterEnumAsString(int enumVal);
const char* WriteSamplerMipmapModeEnumAsString(int enumVal);
const char* WriteSamplerAddressModeEnumAsString(int enumVal);
const char* WriteBorderColorEnumAsString(int enumVal);
const char* WriteDescriptorTypeEnumAsString(int enumVal);
const char* WriteAttachmentLoadOpEnumAsString(int enumVal);
const char* WriteAttachmentStoreOpEnumAsString(int enumVal);
const char* WritePipelineBindPointEnumAsString(int enumVal);
const char* WriteCmdBufferLevelEnumAsString(int enumVal);
const char* WriteIndexTypeEnumAsString(int enumVal);
const char* WriteSubpassContentsEnumAsString(int enumVal);
const char* WriteColorSpaceKHREnumAsString(int enumVal);
const char* WritePresentModeKHREnumAsString(int enumVal);
const char* WriteDebugReportObjectTypeEXTEnumAsString(int enumVal);
const char* WriteDebugReportErrorEXTEnumAsString(int enumVal);

std::string WritePointerAsString(const void* ptr);
std::string WriteUint64AsString(uint64_t value);

std::string DecomposeFormatFeatureFlagsEnumAsString(UINT flags);
std::string DecomposeImageUsageFlagsEnumAsString(UINT flags);
std::string DecomposeImageCreateFlagsEnumAsString(UINT flags);
std::string DecomposeSampleCountFlagsEnumAsString(UINT flags);
std::string DecomposeQueueFlagsEnumAsString(UINT flags);
std::string DecomposeMemoryPropertyFlagsEnumAsString(UINT flags);
std::string DecomposeMemoryMapFlagsEnumAsString(UINT flags);
std::string DecomposeSparseImageFormatFlagsEnumAsString(UINT flags);
std::string DecomposeSparseMemoryBindFlagsEnumAsString(UINT flags);
std::string DecomposeFenceCreateFlagsEnumAsString(UINT flags);
std::string DecomposeQueryPipelineStatisticFlagsEnumAsString(UINT flags);
std::string DecomposeQueryResultFlagsEnumAsString(UINT flags);
std::string DecomposeBufferUsageFlagsEnumAsString(UINT flags);
std::string DecomposeBufferCreateFlagsEnumAsString(UINT flags);
std::string DecomposeImageAspectFlagsEnumAsString(UINT flags);
std::string DecomposeColorComponentFlagsEnumAsString(UINT flags);
std::string DecomposeDescriptorPoolCreateFlagsEnumAsString(UINT flags);
std::string DecomposePipelineCreateFlagsEnumAsString(UINT flags);
std::string DecomposeShaderStageFlagsEnumAsString(UINT flags);
std::string DecomposeCullModeFlagsEnumAsString(UINT flags);
std::string DecomposeAttachmentDescriptionFlagsEnumAsString(UINT flags);
std::string DecomposePipelineStageFlagsEnumAsString(UINT flags);
std::string DecomposeAccessFlagsEnumAsString(UINT flags);
std::string DecomposeDependencyFlagsEnumAsString(UINT flags);
std::string DecomposeCmdPoolCreateFlagsEnumAsString(UINT flags);
std::string DecomposeCmdPoolResetFlagsEnumAsString(UINT flags);
std::string DecomposeCmdBufferUsageFlagsEnumAsString(UINT flags);
std::string DecomposeStencilFaceFlagsEnumAsString(UINT flags);
std::string DecomposeQueryControlFlagsEnumAsString(UINT flags);
std::string DecomposeCommandBufferResetFlagsEnumAsString(UINT flags);
std::string DecomposeSurfaceTransformFlagBitsKHRFlagsEnumAsString(uint32 flags);
std::string DecomposeCompositeAlphaFlagsEnumAsString(uint32 flags);
std::string DecomposeDebugReportFlagsEXTEnumAsString(uint32 flags);
}

#ifdef _DEBUG
    #ifdef WIN32
        /// Break on condition
        #define VKT_ASSERT(__expr__) if (!(__expr__)) __debugbreak();

        /// Always break
        #define VKT_ASSERT_ALWAYS() __debugbreak();

        /// Break on a code path which needs implementation
        #define VKT_ASSERT_NOT_IMPLEMENTED() __debugbreak();
    #else
        /// Break on condition
        #define VKT_ASSERT(__expr__) if (!(__expr__)) raise(SIGTRAP);

        /// Always break
        #define VKT_ASSERT_ALWAYS() raise(SIGTRAP);

        /// Break on a code path which needs implementation
        #define VKT_ASSERT_NOT_IMPLEMENTED() raise(SIGTRAP);
    #endif // WIN32
#else
    /// Break on condition
    #define VKT_ASSERT(__expr__)

    /// Always break
    #define VKT_ASSERT_ALWAYS()

    /// Break on a code path which needs implementation
    #define VKT_ASSERT_NOT_IMPLEMENTED()
#endif

#endif
