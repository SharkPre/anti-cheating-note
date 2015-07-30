# 游戏安全笔记

## 客户端

### 内存数值修改
坏人通过查找内存中的数值(比如金钱数999)、或者在数值变化前后做内存对比(比如血量从100掉到80)等方法来定位你变量的位置，然后进行内存修改或锁定。

对应方法：
* 不要"明文"储存重要的数值
* 把数值放到多处备份，每次都要对所有备份进行一致性校验。
* 记录数值的变化历史，检测不正常的数值变化。

*参考ProtectedNumber.cpp的一些数值处理方法*

### 时间变速
坏人通过劫持系统时间相关的函数(比如gettimeofday)，对时间值增加或者减少，达到加速/减速的效果。

对应方法：
* 如果可以进行网络对时，那么就可以轻易检测到客户端被变速。
* 单机检测的方法是，单起一个线程，记录当前时间，然后sleep，记录醒来后的时间，如果记录到的时间差和sleep的数字相差较大，而且多次检测都有问题的话，那基本可以认为客户端被变速了。
*伪代码仅供参考*
```c++
void threadFunc () {
	int st = 1000;
	int tolerance = 500;
	int maxCount = 5;

	int count = 0;
	long now = getms ();
	while (true) {
		sleep (st);
		int diff = abs (getms () - now - tolerance);
		if (diff > tolerance)
			count++;
		else
			count = 0;
		if (count > maxCount) {
			// cheat detected!
		}
		now = getms ();
	}
}
```

### 代码保护
* c/c++的代码要移除所有debug symbol
* c#/java的代码一定要混淆
* 脚本用二进制保存
* 一些敏感的函数名字改掉，比如一个修改金钱的函数ModMoney，可以通过define大法来保持可读性和安全性
```c++
#define ModMoney ascivjkdf
void Player::ModMoney (int m)
```

## 服务器

### 不要相信客户端发过来的数值 不要相信客户端发过来的数值 不要相信客户端发过来的数值
