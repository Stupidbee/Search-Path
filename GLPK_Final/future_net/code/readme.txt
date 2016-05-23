1. 采用了glpk-4.60开源库，使用协议为GNU General Public License(GPL)协议
开源库的源码包放在代码目录下，文件名为glpk-4.60.tar.gz, 该库为GNU开发，可自由
传播开源。编译生成的静态链接库文件位于代码目录下的lib目录中，文件名为libglpk.a,该开源库的下载地址为http://ftp.gnu.org/gnu/glpk/glpk-4.59.tar.gz

2. 团队使用的部分参考资料和代码来源于互联网，其中蚁群算法的模板来源于以下链接
http://blog.csdn.net/jjj19891128/article/details/25230089

3. 代码使用了官方提供的SDK进行打包，使用了官方的batch.sh打包的，
但是修改了官方提供的SDK中的CMakeLists.txt文件，增加了链接库的选项。

4. 如果修改了CMakeLists.txt文件对官方编译生成可执行文件有影响的话，
请在code目录中使用一下编译命令，即可生成future_net可执行文件：
g++ *.cpp ./lib/libglpk.a --std=c++11 -o future_net
