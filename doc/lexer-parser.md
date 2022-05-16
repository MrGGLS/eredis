# Lexer and Parser

## 1 支持的语法

### 配置

<!-- - `config set xxx` -->

### Key 操作

1. `key *`: 查看当前库所有key
2. `exists key`: 判断某个key是否存在
3. `type key`: 查看key的类型e
4. `del key`: 删除指定的key数据
5. `dbsize`: 查看当前数据库中key的数量
6. `flushdb`: 清空当前数据库
7. `flushall`: 清空所有数据库
8. `select [n]`: 进入第n个数据库(总共16个)

### 类型

#### String

1. `set [key] [value]`: 添加键值对
2. `get [key]`: 获取当前 key 对应的 value
3. `strlen [key]`：获取 value 长度
4. `append [key] [value]`：在原来 value 的基础上追加子串，返回总的字符串长度
5. `getrange [key] [start] [end]`: 获取 value[start..end]

#### List
#### Set
#### Hash

### 数据保存

1. `save`: 保存数据



