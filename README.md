# 游戏安全笔记

本文是关于游戏安全方面的简单笔记。
了解并实践下面的知识，并不代表你的游戏就能避免被破解，要有一个清醒的认识：这是一个永远魔高一尺的世界。

## 客户端

### 内存数值修改
坏人通过查找内存中的数值(比如金钱数999)、或者在数值变化前后做内存对比(比如血量从100掉到80)等方法来定位你变量的位置，然后进行内存修改或锁定。

对应方法：
* 不要"明文"储存重要的数值
* 把数值放到多处备份，每次都要对所有备份进行一致性校验。
* 记录数值的变化历史，检测不正常的数值变化。

*参考ProtectedNumber.cpp的一些数值处理方法*
```c++
class ProtectedNumber {
public:
	ProtectedNumber (int n) {
		mMask = 0;
		mMaskB = 0;
		
		mIndex = 0;
		mValueB = new int[10];
		memset (mValue, 0, sizeof (mValue));
		memset (mValueB, 0, sizeof (mValue));

		for (int i = 0; i < sizeof (int); i++) {
			mMask |= (rand () % 255 + 1) << (i * 8);
			mMaskB |= (rand () % 255 + 1) << (i * 8);
		}

		Set (n);
	}

	~ProtectedNumber (){
		delete mValueB;
	}

	void Set (int n){
		mValue[mIndex] = n ^ mMask;
		mValueB[mIndex] = n ^ mMaskB;
		mIndex++;
		if (mIndex >= 10)
			mIndex = 0;
	}

	int Get (){
		int v = mValue[mIndex] ^ mMask;
		if (v != (mValueB[mIndex] ^ mMaskB)) {
			// cheat detected!
			return 0;
		}
		return v;
	}

private:
	int mMask;
	int mMaskB;

	int mIndex;
	int mValue[10];
	int *mValueB;
};
```

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
坏人通过反汇编软件或反编译软件来分析你的代码

对应方法：
* c/c++的代码要移除所有debug symbol
* c#/java的代码一定要混淆
* 脚本用二进制保存
* 代码里的字符串是明文的，不要把加密密钥写在代码里
* 一些敏感的函数名字改掉，比如一个修改金钱的函数ModMoney，可以通过define大法来保持可读性和安全性
```c++
#define ModMoney ascivjkdf
void Player::ModMoney (int m)
```

### IAP劫持
坏人通过劫持iap相关的函数，让客户端以为iap购买成功了，实际上iap请求根本没有发到苹果服务器

对应方法：
* 务必[通过服务器去验证iap收据](https://developer.apple.com/library/ios/releasenotes/General/ValidateAppStoreReceipt/Chapters/ValidateRemotely.html#//apple_ref/doc/uid/TP40010573-CH104-SW1)

### 客户端文件修改
坏人会修改你的执行文件，插入他的代码，或删除你的代码，会修改你的资源文件，会修改你的配置文件

对应方法：
* 敏感的配置文件不要放在明显的目录，不要用明显的文件名，加密(例如XXTEA)后藏到资源文件目录里边去
* ios上你是可以读取程序的代码段内存镜像的([Mach-O文件格式介绍](https://developer.apple.com/library/mac/documentation/DeveloperTools/Conceptual/MachORuntime/Reference/reference.html))，对载入后的代码段做hash，发到服务器做校验吧


## 服务器
没啥好说的，记住一点
###不要相信客户端发过来的数值！ 不要相信客户端发过来的数值！ 不要相信客户端发过来的数值！
