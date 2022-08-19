# HouseManager 职工住房管理系统
## 解决数据管理问题

首先最主要的就是数据的读取问题，第一步要做的就是文件解析。

*感觉自己实现了序列化和反序列化哈哈哈哈哈*

### 一、自定义规范的文件结构：

`[start]worker_id:xxx|worker_name:xxx|worker_age:xxx|date:xxx|degree:xxx|title:xxx|area:xxx|address:xxx|{name:xxx&gender:xxx&age:xxx&appellation:xxx}{...}...[end]`

大括号之前的存储职工的基本信息，每个大括号表示一个亲属，大括号内的内容存储该亲属的基本信息，可以有0或多个大括号。

记录以文本形式存储，一条记录存储为一行

###  二、定义存储数据的结构

首先是`Database`类，一个实例对应一个数据库。`Database`类有成员变量`QVector<Worker> workers`，这个向量里面的每个元素对应一个`Worker`类实例，也就是表示一个职工。提供构造函数，作用是读入一个数据文件，解析内容并存储到成员变量`workers`内。提供`save2File`方法，作用是将当前`Database` 实例所存储的内容按照规范的文件结构存储到数据文件内

`Worker`类含有8个基本的职工信息成员变量，还有一个成员变量`QVector<Relative> relatives`，里面每个元素对应这个职工的一个亲属。该类提供一个`void Worker::setRelatives(QVector<Relative> relatives)`方法，传入存有亲属信息的向量，对自己的`relatives`成员变量赋值。提供构造函数，传入8个基本职工信息参数并一一赋值

`Relative`类，含有4个亲属信息成员变量`name gender age appellation`。提供构造函数，传入4个基本职工信息参数并一一赋值

*踩坑：使用QVector<MyClass>的话必须要给出默认构造函数，否则编译不能通过，我当时调试了很久才发现这个问题。*

我将这些功能是实现写在了自己给出的文件`mytools.h`和`mytools.cpp`内，通过include的方式在其他cpp内使用，很方便。

*踩坑：最开始没有接口实现分离，Qt在编译的时候出现了重复包含的错误，我原来没有刻意分离的习惯，因此一直找不到编译报错重复包含原因，后来在stackoverflow和QtForum上找到了解决办法，我分离了之后就顺利解决了问题。Qt一定要分离，不然编译的时候会出现奇奇怪怪的问题*

### 三、解析文件内容

`Database`类的构造函数伪代码如下

```
datafile = open_file("datafile_path")
while not datafile.reach_end():
	current_line = read_one_line(datafile)
	one_worker = parse_the_line(current_line)
	workers.push_back(one_worker)
// after the loop, all the infomation stored in datafile
// will be scanned into vector "all_workers"
datafile.close()
```

重点关注这个`parse_the_line()`函数的实现。首先我们读到的文件必定是一个字符串，类型是QString，那么它必定可以用正则去匹配到，Qt、提供了正则匹配的方法以及正则类QRegExp，那么按照定下的的标准文件规范来查找匹配就一定能匹配到目标内容。

为什么要正则匹配那么复杂的方式呢？因为这具有良好的扩展性，如果以后职工或亲属需要添加属性，比如加一个薪酬的属性，那么原有的代码完全不需要修改，只需要新添加关于新属性的读取正则即可，这样扩展性就非常的好。

对于职工基本信息部分的匹配，我实现了`QString re_match_info(QString rawstr,QRegExp re)`函数，接受从文件中读取的一行QString字符串，接受一个正则表达式对象，返回匹配结果。例如，我要获取到worker_name，而读取的那一行信息中包含`xxxxx|worker_name:localh0st|xxxx`的字样，特征是`worker_name:`开头，分隔符`|`结尾，那么这个正则表达式对象就要这么写：`QRegExp("worker_name:[^|]*")`，匹配获取到`worker_name:localh0st`，字符串截取之后获得真正的内容`localh0st`并作为返回值返回。同样的，其余七个职工基本信息变量也可以获取到。调用`Worker`构造函数，组装出目前这个worker。

紧接着要做的是解析亲属信息，调用自定义函数`QVector<Relative> re_match_relatives(QString rawstr)`，传入当前行的字符串，调用自定义函数`QSet<QString> getAllMatchResults(const QString text,const QString regexp)`获取到当前行所有的大括号，每个括号当作一个元素存在QSet内，遍历该Qset，对每一个元素调用4次`re_match_info`，获取到每一个亲属的4个基本信息，最终成功组装出亲属信息向量并当成返回值返回

返回值被当前worker的`setRelatives`方法接收，完成对亲属信息的解析和存储，最后将这个worker对象存储入当前`Database`实例的`workers`成员变量内。当循环结束，整个文件的每一行都被解析完成，最后文件关闭，整个解析过程结束。

## 窗体设计

* 主窗口mainwindow用于显示职工信息
* 下拉菜单提供以下选项（增删改查）：
  1. 查询亲属信息
  2. 修改/删除 职工/亲属 信息
  3. 添加职工
  4. 为某一个职工添加亲属

### 查询亲属信息（查）

通过一个下拉菜单选择职工，点击确认按钮后获取下拉菜单的currentIndex，由于下拉菜单的加载顺序和数据库里面的存储顺序严格一致，因此通过currentIndex到`Database`实例中可以找到对应记录，然后显示亲属信息

### 修改/删除 职工/亲属 信息（删改）

这里实现了一个`void::Database::save2File(QString path)`方法，也就是将当前`Database`实例里面存储的信息写入到文件内。，这个难度不大，就是按照格式拼接字符串然后写入文件就行。那么如何实现删改功能呢？

首先我们新建一个`Database`实例`new_db`，并确定需要删改的职工（和亲属）的currentIndex，然后遍历原来的`db`，当还没有遍历到需要删改的职工的时候，直接从`db`复制当前的职工对象到`new_db`里面，当遍历到需要删改的职工的时候，更具获取到的信息组装出修改过后的职工对象，然后直接添加到`new_db`里面。最后调用`new_db.save2File()`方法，覆盖原来数据库文件，写入新内容

在编辑的时候，我们希望点击选择职工的时候将基本信息先显示出来，然后再做编辑，这其中对于QDateEdit组件来说稍嫌麻烦。我采用了提取代表生日/入职日期的QString中的年月日，构造QDate对象，使用QDate对象调用`setDate`函数来设置QDateEdit，成功解决以前没有解决的问题

*踩坑：默认编码是unicode写入，这对于中文并不友好。之前测试的时候我都是按照英文来的，没有问题，但后来中文测试的时候发现不正常。一开始我还以为是读文件的问题，后来仔细一查发现是写文件，写文件之前的最后一步拼接完成的字符串都是没问题的，那只能是写文件的时候出了问题。网上一查可能是编码问题，在qt官方文档找到了setCodec函数的使用示例，将编码设置为utf-8，最终解决了写文件中文乱码问题。*

### 添加职工、为某一个职工添加亲属（增）

已有的职工对象直接从`db`复制到`new_db`，要新增的职工组装出一个新的`Worker`实例并添加到`new_db`，要新增的亲属直接组装出一个新的`Relative`实例，并添加到该职工的`relatives`属性里，最后调用`new_db.save2File()`即可。亲属年龄采用输入的方式，通过`setVaidator`函数，传入正则表达式`[1-9][0-9]*`限制为只能输入数字，考虑到积分信息里面需要动态计算年龄分，因此职工年龄就采用输入生日动态计算的方式。考虑到住房面积只能是数字，且有可能是0，因此通过`setVaidator`函数，传入正则表达式`()[1-9][0-9])|(0)*`，限制输入只能是数字或0。

### 统计积分信息

这个比较容易实现，通过遍历每一个职工，统计其对应的职称分、工龄分、年龄分、学龄分、现住房分：每平米0.1分；家庭成员分，并相加求和，显示在tableview上即可，最后设置tableview可排序即可。

## 后期程序设计修改

之前的数据文件路径和名称统统都是写死的，这显然不合理，我的解决办法是在mytools中添加一个信息类Info，Info包含一个成员变量datafile_path。在mainwindow中加入一个Info类的实例，用于存储datafile_path。在每个窗体的构造函数参数列表里面新增`const Info & i`，采用初始化列表的方式赋值，用于mainwindow到子窗口的传递信息。在mainwindow添加下拉菜单功能`选择数据文件`，这个功能可以设置mainwindow的Info实例中的datafile_path，之后在弹出子窗口的时候传递mainwindow的Info实例即可。至于为什么不传递QString而是单独新建一个Info类，是因为为了可扩展性，万一以后还要传递其他的参数，那样的话代码几乎不用修改。

## 修改建议

1. 提升交互体验，使之更加人性化
2. 数据存储可以改成json文件，读写方便。







## 测试报告

**使用数据：**

[start]worker_id:SEU2022|worker_name:老王|worker_age:1970/6/12|date:2001/5/1|worker_degree:硕士|title:副教授|area:160|address:江苏南京|{name:王喜顺&gender:男&age:52&appellation:父亲}{name:李姐&gender:女&age:52&appellation:母亲}[end]

[start]worker_id:SEU2009|worker_name:老张|worker_age:1986/8/29|date:1990/11/6|worker_degree:本科|title:讲师|area:106|address:江苏苏州|{name:张姐&gender:女&age:26&appellation:妹妹}[end]

[start]worker_id:SEU2006|worker_name:老李|worker_age:1996/5/12|date:2006/9/1|worker_degree:硕士|title:助教|area:0|address:无|{name:test&gender:女&age:13&appellation:testtttt}[end]

[start]worker_id:SEU2001|worker_name:老徐|worker_age:1968/6/6|date:1980/11/21|worker_degree:博士|title:教授|area:189|address:江苏南京|{name:王姐&gender:女&age:55&appellation:配偶}[end]

测试查询亲属信息：均能正常显示

测试添加职工信息：均能正常添加

测试添加亲属信息：添加完之后进行查询，均能查到有效记录，测试正常

测试编辑职工信息功能：逐一修改职工信息和亲属信息，各个字段修改均成功，未出现不合法的输入和显示结果。

测试删除功能：逐一删除亲属和职工，均发现能够正常删除。

以上各个功能的执行结果均在数据文件中得到验证。

测试显示职工积分信息功能：均能正常显示且计算正确，支持排序。

测试退出功能，一键退出成功。

最后我通过Qt自带的工具windeployqt获取到我写的这个程序的release版本，并且通过Enigma Virtual Box工具讲dll等文件和exe打包到一起，生成一个可以独立运行的exe文件，这样会比较方便

## 学习小结

其实Qt学习过程中遇到了很多的问题，刚上手的时候会发现，窗体添加了控件但是cpp的ui指针却不能访问到，解决办法是重新构建项目。很多时候程序遇到小问题，可以采用`qDebug()<<`的方式输出查看。其实Qt学习除了基本的入门教程，其他时候都是面向Google Stackoverflow QtForum编程。

我遇到过的几个卡了比较久的问题：

1. 使用QVector<MyClass>的话必须要给出默认构造函数，否则编译不能通过，我当时调试了很久才发现这个问题。
2. 最开始没有接口实现分离，Qt在编译的时候出现了重复包含的错误，我原来没有刻意分离的习惯，因此一直找不到编译报错重复包含原因，后来在stackoverflow和QtForum上找到了解决办法，我分离了之后就顺利解决了问题。Qt一定要分离，不然编译的时候会出现奇奇怪怪的问题
3. 在写入文件的时候，默认编码是unicode写入，这对于中文并不友好。之前测试的时候我都是按照英文来的，没有问题，但后来中文测试的时候发现不正常。一开始我还以为是读文件的问题，后来仔细一查发现是写文件，写文件之前的最后一步拼接完成的字符串都是没问题的，那只能是写文件的时候出了问题。网上一查可能是编码问题，在qt官方文档找到了setCodec函数的使用示例，将编码设置为utf-8，最终解决了写文件中文乱码问题

有一点感想：网上给出的代码，实现一个功能可能又好多种不同的写法，有的时候你参考的代码很复杂，写完了才发现还有一种更加简单的实现方式；有的时候完全没有现成的可以拿来用的，比如我在修改职工信息窗口中，在QDateEdit里面读取生日和入职日期并显示，这个功能久完全没有现成的，这个时候就可以通过搜索，寻找可能的解决方案，比如我只找到了QDateEdit设置当前日期，我发现它调用setDate函数并传入了一个返回当前日期QDate对象的函数，那么我想，能不能自己构造一个QDate对象然后传入呢？我就去找QDate的构造函数的用法，最终证明我的解决办法是可行的。

所有其实写代码的过程本身就是一种学习的过程，寻找解决方案的时候也在不知不觉地提升自己，通过本次的Qt程序设计我学到了很多普通C++课堂学不到的东西。
