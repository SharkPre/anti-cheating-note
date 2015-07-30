#include <stdlib.h>
#include <assert.h>

class ProtectedNumber {
public:
	ProtectedNumber (int n) {
		mMask = 0;
		mMaskB = 0;
		mValueB = new int (0);

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
		mValue = n ^ mMask;
		*mValueB = n ^ mMaskB;
	}

	int Get (){
		int v = mValue ^ mMask;
		if (v != ((*mValueB) ^ mMaskB)) {
			// cheat detected!
			return 0;
		}
		return v;
	}

private:
	int mMask;
	int mMaskB;

	int mValue;
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
