#include "Pugiparam.h"

_mData mData;

Pugiparam pparam("setup.xml", &mData);

int main()
{
	_Parameter prmp;
	printf("Start...\n");
	pparam.getParam();
	pparam.setParam("other", "Mm", 57);
	cout << endl;
        //for (int i = 0; i < 3; i++) {
	//	pparam.viewParam(&prmp, i);
	//	std::cout << prmp.id << " " << prmp.prp[0].Name << " " << prmp.prp[0].value << " " << prmp.prp[1].Name << " " <<  prmp.prp[1].value << "\n";
	//}
}
