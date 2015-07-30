#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

int main()
{
	ProtectedNumber pn (10);
	assert (pn.Get () == 10);
	pn.Set (15);
	assert (pn.Get () == 15);
	return 0;
}
