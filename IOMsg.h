#pragma once
class CIOMsg
{
public:
	CIOMsg(void);
	~CIOMsg(void);

 	
	void KeyPress(int vKeyCoad);

	void KeyPress(int vKeyCoad, int times);

	void KeyPressDown(int vKeyCoad);

	void KeyPressUp(int vKeyCoad);

private:
	int KBC_KEY_CMD;  
	int KBC_KEY_DATA;  
	void KBCWait4IBE();
};

