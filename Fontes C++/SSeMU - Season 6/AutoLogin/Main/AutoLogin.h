#pragma once

#define pTextThisAL				((LPVOID(*)()) 0x0041FE10)
#define pDrawTextAL				((int(__thiscall*)(LPVOID This, int PosX, int PosY, LPCTSTR Text, int Width, int Height, LPINT Align, int unknow)) 0x00420150)

class cAutoLogin
{
public:
	void Load();
	bool WriteString(const std::string& name, const std::string& value);
	bool WriteDword(const std::string& name, DWORD value);
	void DoRequestLogin();
	void DoLoginWinCreate(int);
	void CButtonAutoLogin(int This, int a2);
	void TextLogin(int X, int Y, LPCTSTR Text);

	BOOL savePass;
	char iUser[32];
	char iPassword[32];
private:

};

extern cAutoLogin AutoLogin;