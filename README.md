# Y86 模拟器

## 项目简介

本项目为"计算机体系结构"课程的大作业，实现了一个 Y86 模拟器, Y86 是一个简化的指令集架构, 来自教材《深入理解计算机系统》。

## 目录结构

- `include`：头文件
- `src`：源文件
- `test`：测试文件
- `answer`：测试文件答案, 包含`yaml`和`json`两种格式
- `test.py`：测试脚本, 用于测试`test`目录下的所有测试文件
- `CMakeLists.txt`：`cmake`配置文件
- `CPU阶段分解.xlsx`: CPU 阶段分解表格, 包含每个指令的各个阶段的操作

## 编译

```bash
cmake .
make
```

可执行文件为`y86_backend`，位于`build`目录下。

## 运行

`y86_backend`默认情况下从标准输入读取指令，从标准输出输出结果。可以通过命令行参数指定输入输出文件。

```bash
./y86_backend < [input_file] > [output_file]
```

todo: 通过命令行参数指定输入输出文件

## 测试

```bash
python3 test.py
```

如果测试通过，会输出`All correct!`，否则会输出错误之处。
