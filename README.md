# Tomasulo CPU Simulator

#### ------------------------------------------------------ livandan 2025.7

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
* 如果接收到分支预测错误信号，丢弃当前task
* 解码
* 获取并更新依赖关系

      得到要读的寄存器的依赖Q1与Q2，把要写的寄存器序号和对应的RoB位置发给RF
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
* 如果接收到分支预测错误信号，丢弃当前Task
* 计算
* 传输

      计算操作：
        -> RoB & RS & LSB: 指令类型、哪个位置、值是多少
      Jalr:
        -> PC: 解除等待状态，下一步跳转到什么位置
      Load:
        -> LSB: 指令类型、计算出的地址、对应的RoB位置（寄存器状态不变所以不用广播）
      Store:
        -> RoB: 指令类型、哪个位置、计算出的地址（不可能有指令依赖Store所以只要修改不用广播）
* 更新

      用被RS设置的新任务更新操作种类、操作数、操作对应RoB编号

### Memory
#### 连接元件
* 组合：PC
* 时序：LSB
#### 运行操作
* 如果RoB上一周期发来分支预测错误信号，强制修改PC到指定位置
* 发送机器码

      如果PC不在等待状态，取出PC处的值，发送给Decoder
      如果PC在等待状态，告诉Decoder下个周期没有需要解码的指令

* 如果当前周期有Load任务，读出指定地址的值，和RoB编号一起发送给RoB与RS和LSB
* 更新

      如果当前周期有Store任务，把数据存入memory

### RoB
#### 连接元件
* 时序：RS, LSB, Decoder
#### 运行操作
* 从head开始检查每条指令的is_ready，直到遇到没有准备好commit的指令停止检查
* 提交

      计算操作：
        -> RF: 写入哪个寄存器、写什么数据
      Branch:
        判断正确：继续操作，给Predictor一个反馈
        判断错误：
          -> 给RoB本身、RS、LSB、Decoder发送分支预测错误信号
          -> 给PC发送分支预测错误信号和跳转目标位置
          -> 给Predictor一个反馈
      Jalr & Restore:
        -> RF: 存什么、存哪里、提交的指令在RoB中的编号
      Store:
        -> LSB: 被提交的指令的RoB编号、要存的数据、Memory中地址
* 传输

      -> Decoder & RS & LSB: 当前tail位置
* 更新

      如果接收到分支预测错误信号：全部清空，下一步task设为空
      如果正常运行：把ALU发来的值填入，把所有依赖这条指令的RoB依赖关系去除并更新值
                 把Decoder发来的信息填入tail的位置，++tail，--size

### RF
#### 连接元件
* 时序：RoB, RS, Decoder
#### 运行操作
......