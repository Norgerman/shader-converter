# shader-converter

GLSL to WGSL package for Web and Node

Web和Node的GLSL到WGSL转换工具包

## HOW TO BUILD
prerequisites
- emscripten
- depot_tools
- python
- cmake
- ninja-build(optional)

1. init submodule
   ```bash
   git submodule update --init --depth 1
   ```
2. fetch dawn dependencies(in dependencies/dawn)
   ```bash
    cp scripts/standalone.gclient .gclient
    gclient sync --no-history
   ```
3. apply patches(in repo root)
   ```bash
   git apply patches/dawn.patch
   ```
4. cmake(create build directory under root，use ninja for example，use make will work to)
   ```bash
   # FOR WEB
   emcmake cmake -DCMAKE_BUILD_TYPE=RELEASE -G Ninja ../
   # FOR NODEJS
   emcmake cmake -DCMAKE_BUILD_TYPE=RELEASE -DENABLE_ENVIRONMENT_NODE=ON -G Ninja ../
   ```
5. build(16 cpus for example)
   ```bash
   ninja shader_converter -j 16
   ```


## 如何编译
准备好以下相关工具
- emscripten
- depot_tools
- python
- cmake
- ninja-build(可选)

1. 初始化submodule
   ```bash
   git submodule update --init --depth 1
   ```
2. 拉取dawn依赖包(dependencies/dawn目录下)
   ```bash
    cp scripts/standalone.gclient .gclient
    gclient sync --no-history
   ```
3. 应用补丁(仓库根目录)
   ```bash
   git apply patches/dawn.patch
   ```
4. 执行cmake(自行创建build目录，此处使用Ninja进行构建，也可以使用make)
   ```bash
   # Web环境
   emcmake cmake -DCMAKE_BUILD_TYPE=RELEASE -G Ninja ../
   # Node环境
   emcmake cmake -DCMAKE_BUILD_TYPE=RELEASE -DENABLE_ENVIRONMENT_NODE=ON -G Ninja ../
   ```
5. 执行构建(以16核为例)
   ```bash
   ninja shader_converter -j 16
   ```
