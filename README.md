# Tomasulo CPU Simulator

## 元件
- Decoder(组合逻辑连Predictor和PC)
- RoB
- RF
- RS
- LSB
- Memory
- ALU

## 各元件操作

### Decoder
#### 连接元件
* 组合：Predictor, PC
* 时序：RoB, RS, LSB
#### 运行操作
* 解码
* 获取并更新依赖关系

      要读的寄存器的依赖Q1与Q2，把要写的寄存器序号和对应的RoB位置发给RF
* 设置下一步PC更新任务

      Branch: 立即丢给Predictor并获取结果，把预测结果和当前PC加到指令里并且告诉PC下一步的位置；
      Jump: 告诉PC下一步的位置
      Jalr: 告诉PC等着（设置状态，等待RoB/RS得到结果解除状态）
* 传输

      -> RoB: 指令类型、要写回的寄存器编号、该寄存器的依赖关系
              （如果操作值是立即数，还要发送立即数并告知is_ready为true）
      -> RS: 指令类型、V1、V2、Q1、Q2、RoB中位置
      -> LSB: 指令类型、
              如果操作是Store: 操作值、目标地址、还未被commit、哪条RoB指令commit的时候可以执行
              如果操作是Load: 操作值（和操作值依赖的RoB序号）、要把取出来的值告诉RoB哪个位置
* 更新

      用被PC设置的新任务更新machine code

### ALU
#### 连接元件
* 时序：RoB, RS, LSB
#### 运行操作
* 计算
* 传输

      计算操作：
        -> RoB & RS & LSB: 指令类型、哪个位置、值是多少
      Load:
        -> LSB: 指令类型、计算出的地址、对应的RoB位置（寄存器状态不变所以不用广播）
      Store:
        -> RoB: 指令类型、哪个位置、计算出的地址（不可能有指令依赖Store所以只要修改不用广播）
* 更新

      用被RS设置的新任务更新操作种类、操作数、操作对应RoB编号

......