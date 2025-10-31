/**
 * reference:
 * https://github.com/KhronosGroup/glslang
 * https://github.com/BabylonJS/twgsl
 */

#include <SPIRV/GlslangToSpv.h>
#include <emscripten.h>
#include <glslang/Public/ShaderLang.h>
#include <sstream>
#include <string>
#include <tint/tint.h>
#include <vector>

const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxMeshOutputVerticesEXT = */ 256,
    /* .maxMeshOutputPrimitivesEXT = */ 512,
    /* .maxMeshWorkGroupSizeX_EXT = */ 32,
    /* .maxMeshWorkGroupSizeY_EXT = */ 1,
    /* .maxMeshWorkGroupSizeZ_EXT = */ 1,
    /* .maxTaskWorkGroupSizeX_EXT = */ 32,
    /* .maxTaskWorkGroupSizeY_EXT = */ 1,
    /* .maxTaskWorkGroupSizeZ_EXT = */ 1,
    /* .maxMeshViewCountEXT = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */
    {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

extern "C" {
void glsl_to_wgsl(const char *glsl, int stage_int, bool gen_debug,
                  glslang::EShTargetLanguageVersion spirv_version,
                  bool disableUniformityAnalysis = false);
extern void return_string(const void *data, int length);
extern void return_error(const void *data, int length);
}

static bool initialized = false;

EMSCRIPTEN_KEEPALIVE
void glsl_to_wgsl(const char *glsl, int stage_int, bool gen_debug,
                  glslang::EShTargetLanguageVersion spirv_version,
                  bool disableUniformityAnalysis) {
    if (glsl == nullptr) {
        auto err = std::string("Input pointer null");
        return_error(err.data(), err.size());
        return;
    }

    if (stage_int != 0 && stage_int != 4 && stage_int != 5) {
        auto err = std::string("Invalid shader stage");
        return_error(err.data(), err.size());
        return;
    }

    EShLanguage stage = static_cast<EShLanguage>(stage_int);

    switch (spirv_version) {
    case glslang::EShTargetSpv_1_0:
    case glslang::EShTargetSpv_1_1:
    case glslang::EShTargetSpv_1_2:
    case glslang::EShTargetSpv_1_3:
    case glslang::EShTargetSpv_1_4:
    case glslang::EShTargetSpv_1_5:
        break;
    default:
        auto err = std::string("Invalid SPIR-V version number");
        return_error(err.data(), err.size());
        return;
    }

    if (!initialized) {
        glslang::InitializeProcess();
        initialized = true;
    }

    glslang::TShader shader(stage);
    shader.setStrings(&glsl, 1);
    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetSpv, spirv_version);

    if (!shader.parse(&DefaultTBuiltInResource, 100, true, EShMsgDefault)) {
        std::stringstream ss;
        ss << "Parse failed\n"
           << shader.getInfoLog() << std::endl;
        auto err = std::move(ss).str();
        return_error(err.data(), err.size());
        return;
    }

    glslang::TProgram program;
    program.addShader(&shader);

    if (!program.link(EShMsgDefault)) {
        std::stringstream ss;
        ss << "Link failed\n"
           << program.getInfoLog() << std::endl;
        auto err = std::move(ss).str();
        return_error(err.data(), err.size());
        return;
    }

    glslang::SpvOptions spvOptions;
    spvOptions.generateDebugInfo = gen_debug;
    spvOptions.optimizeSize = false;
    spvOptions.disassemble = false;
    spvOptions.validate = false;

    std::vector<uint32_t> spirv;

    glslang::GlslangToSpv(*program.getIntermediate(stage), spirv, nullptr, &spvOptions);
    tint::wgsl::writer::Options options = {};
    options.allow_non_uniform_derivatives = disableUniformityAnalysis;

    auto result = tint::SpirvToWgsl(spirv, options);

    if (result != tint::Success) {
        std::stringstream ss;
        ss << result.Failure() << std::endl;
        auto err = std::move(ss).str();
        return_error(err.data(), err.size());
        return;
    }

    auto wgsl = result.Get();

    return_string(wgsl.data(), wgsl.size());
}