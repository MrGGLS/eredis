# EASY-REDIS

EasyRedis 是根据 Redis 进行设计的一款轻量级键值型数据库。

数据存储上，EasyRedis提供了 Redis 中的String和List两种类型的数据存储，其中String类型能够根据输入自动转型成int和double类型。此外，EasyRedis也提供了键值对过期的功能，能够为某些特定的键值对设置过期时间。EasyRedis采用了与Redis类似的数据存储结构，采用`dump.erdb`存储二进制数据，并提供定时保存和客户主动保存两种机制。

通信模块上，EasyRedis分别实现了Server端和Client端，其中Server端使用C++实现，Client端使用Python实现。针对不同的宿主系统，Server端采用不同的异步I/O实现，例如macOS/BSD下的kqueue、Windows下的select，从性能上来说，能够最多支持1024个客户机同时接入。

用户界面上，EasyRedis使用了Python实现，提供代码补全、历史代码提示等功能。

## 1 组件设计：

1. 词法、语法分析器（doc/lexer-Parser.md）
2. 语法树生成（doc/grammer.md）
3. 语法树解析（doc/Parser-node.md）
4. 数据处理系统（doc/data-manage.md）
5. 输入输出接口（doc/io-interface.md）

## 2 如何开发

### 2.1 建立环境

#### xmake

本项目使用xmake构建，首先需要安装xmake：

安装方式见：[https://xmake.io/#/zh-cn/guide/installation](https://xmake.io/#/zh-cn/guide/installation)

#### IDE 插件

##### 1. IDEA / CLion

搜索安装 xmake 插件，如下图：

![image-20220421134214833](https://raw.githubusercontent.com/MrZLeo/Image/main/uPic/2022/04/21/image-20220421134214833PoHcQy.png)


##### 2. VS Code

搜索安装 xmake 插件，如下图：

![image-20220421134253371](https://raw.githubusercontent.com/MrZLeo/Image/main/uPic/2022/04/21/image-20220421134253371eTqQb1.png)

#### 编码规范

本项目使用 **基于WebKit** 的编码规范，项目中含有正确配置的 `.clang-format` 文件，使用 `clang-format` 能够正确地格式化代码格式。

#### 编译器

本项目使用 clang 编译器，目前的版本为最新版本 13.0.1，请不要使用 MSVC（VS 集成环境），可能会导致编译错误。

> 注意⚠️：本项目另提供cmake环境，Windows下请使用cmake编译

## 3. 支持的语法

### Key 操作

1. `keys *`: 查看当前库所有key
2. `exists [key]`: 判断某个key是否存在
3. `type [key]`: 查看key的类型e
4. `del [key]`: 删除指定的key数据
5. `dbsize`: 查看当前数据库中key的数量
6. `flushdb`: 清空当前数据库
7. `flushall`: 清空所有数据库
8. `select [n]`: 进入第n个数据库(总共16个)
9. `expire [key] [time]`: 给key添加过期时间，单位s
10. `setex [key] [time] [v]`: 在设置key的同时添加过期时间，单位s
11. `ttl [key]`: 检查key还有多少秒过期

### 类型

#### String

1. `set [key] [value]`: 添加键值对
2. `get [key]`: 获取当前 key 对应的 value
3. `strlen [key]`：获取 value 长度
4. `append [key] [value]`：在原来 value 的基础上追加子串，返回总的字符串长度
5. `getrange [key] [start] [end]`: 获取 value[start..end]
6. `incr [key]`: value 的值 +1
7. `decr [key]`: value 的值 -1

#### List

1. `lpush/rpush [k] [v1] [v2] ...`：在k的左边或右边插入数据
2. `lrange [k] [start] [end]`：获取指定范围内的数据（0 代表左边第一个，-1 代表右边第一个）
3. `lpop/rpop [k]`：从左边/右边取出数据，取完所有数据后键消失
4. `lindex [k] [index]`：按照索引获得元素
5. `llen [k]`：获得列表长度
6. `lset [k] [index] [v]`：将对应下标元素替换为v


### 数据保存

1. `save`: 保存数据
