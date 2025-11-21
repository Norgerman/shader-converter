
})(_module, _exports);

const Module = _module.exports;

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

            const { shader_stage_int, spirv_version_int } = get_spirv_spec(shader_stage, spirv_version);
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


