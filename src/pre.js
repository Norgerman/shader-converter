
function get_spirv_spec(shader_stage, spirv_version) {
    let shader_stage_int; // EShLanguage
    switch (shader_stage) {
        case 'vertex': shader_stage_int = 0; break;
        case 'fragment': shader_stage_int = 4; break;
        case 'compute': shader_stage_int = 5; break;
        default:
            throw new Error("shader_stage must be 'vertex', 'fragment', or 'compute'.");
    }

    spirv_version = spirv_version || '1.0';

    let spirv_version_int; // EShTargetLanguageVersion
    switch (spirv_version) {
        case '1.0': spirv_version_int = (1 << 16) | (0 << 8); break;
        case '1.1': spirv_version_int = (1 << 16) | (1 << 8); break;
        case '1.2': spirv_version_int = (1 << 16) | (2 << 8); break;
        case '1.3': spirv_version_int = (1 << 16) | (3 << 8); break;
        case '1.4': spirv_version_int = (1 << 16) | (4 << 8); break;
        case '1.5': spirv_version_int = (1 << 16) | (5 << 8); break;
        case '1.6': spirv_version_int = (1 << 16) | (6 << 8); break;
        default:
            throw new Error("spirv_version must be '1.0' ~ '1.6'.");
    }

    return {
        shader_stage_int,
        spirv_version_int
    }
}

const _exports = {};
const _module = { exports: _exports };

(function (module, exports) {
