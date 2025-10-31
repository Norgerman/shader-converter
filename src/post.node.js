
const { Buffer } = require('node:buffer');
const wasm = {};
Module(wasm);

function buildNodeModule() {
    var wgsl = "";
    var error = undefined;
    if (!wasm._return_string_callback) {
        wasm._return_string_callback = (data, length) => {
            wgsl = Buffer.from(wasm.HEAPU8.buffer, data, length).toString('utf-8');
        };
    }

    if (!wasm._return_string_callback) {
        wasm._return_string_callback = (data, length) => {
            wgsl = Buffer.from(wasm.HEAPU8.buffer, data, length).toString('utf-8');
        };
    }
    if (!wasm._return_error_callback) {
        wasm._return_error_callback = (data, length) => {
            error = Buffer.from(wasm.HEAPU8.buffer, data, length).toString('utf-8');
        };
    }

    return {
        convertGLSLToWGSL(code, shader_stage, gen_debug, spirv_version, disableUniformityAnalysis) {
            gen_debug = !!gen_debug;

            wgsl = "";
            error = undefined;
            const length = Buffer.byteLength(code, 'utf-8');
            const ptr = wasm._malloc(length + 1);
            const buffer = Buffer.from(wasm.HEAPU8.buffer, ptr, length + 1);
            buffer.write(code, 'utf-8');
            buffer[length] = 0;

            var shader_stage_int; // EShLanguage
            switch (shader_stage) {
                case 'vertex': shader_stage_int = 0; break;
                case 'fragment': shader_stage_int = 4; break;
                case 'compute': shader_stage_int = 5; break;
                default:
                    throw new Error("shader_stage must be 'vertex', 'fragment', or 'compute'.");
            }

            spirv_version = spirv_version || '1.0';
            var spirv_version_int; // EShTargetLanguageVersion
            switch (spirv_version) {
                case '1.0': spirv_version_int = (1 << 16) | (0 << 8); break;
                case '1.1': spirv_version_int = (1 << 16) | (1 << 8); break;
                case '1.2': spirv_version_int = (1 << 16) | (2 << 8); break;
                case '1.3': spirv_version_int = (1 << 16) | (3 << 8); break;
                case '1.4': spirv_version_int = (1 << 16) | (4 << 8); break;
                case '1.5': spirv_version_int = (1 << 16) | (5 << 8); break;
                default:
                    throw new Error("spirv_version must be '1.0' ~ '1.5'.");
            }
            wasm._glsl_to_wgsl(ptr, shader_stage_int, gen_debug, spirv_version_int, !!disableUniformityAnalysis);
            wasm._free(ptr);
            if (error) {
                throw error;
            }
            return wgsl;
        }
    }
}

module.exports = buildNodeModule();


