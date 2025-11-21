})(_module, _exports);

const Module = _module.exports;

export default (() => {
    const initialize = (wasmPath) => {
        wasmPath = wasmPath || 'shader_converter.wasm'
        return Module({
            locateFile() {
                return wasmPath;
            }
        }).then(wasm => {
            var wgsl = "";
            var error = undefined;
            const textDecoder = new TextDecoder();
            const textEncoder = new TextEncoder();
            if (!wasm._return_string_callback) {
                wasm._return_string_callback = (data, length) => {
                    wgsl = textDecoder.decode(wasm.HEAPU8.subarray(data, data + length));
                };
            }
            if (!wasm._return_error_callback) {
                wasm._return_error_callback = (data, length) => {
                    error = new Error(wasm.HEAPU8.subarray(data, data + length));
                };
            }
            var convertGLSLToWGSL = (code, shader_stage, gen_debug, spirv_version, disableUniformityAnalysis) => {
                gen_debug = !!gen_debug;

                wgsl = "";
                error = undefined;
                const length = wasm.lengthBytesUTF8(code);
                const ptr = wasm._malloc(length + 1);
                const stat = textEncoder.encodeInto(code, wasm.HEAPU8.subarray(ptr, ptr + length));
                wasm.HEAPU8[ptr + stat.written] = 0;

                const { shader_stage_int, spirv_version_int } = get_spirv_spec(shader_stage, spirv_version);
                wasm._glsl_to_wgsl(ptr, shader_stage_int, gen_debug, spirv_version_int, !!disableUniformityAnalysis);
                wasm._free(ptr);
                if (error) {
                    throw error;
                }
                return wgsl;
            };
            return {
                convertGLSLToWGSL,
            };
        })
    };

    let instance;
    return (wasmPath) => {
        if (!instance) {
            instance = initialize(wasmPath);
        }
        return instance;
    };
})();
