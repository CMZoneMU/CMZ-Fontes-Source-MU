cAutoLogin AutoLogin;

void cAutoLogin::Load()
{
	HKEY hKey;

	DWORD dwDisp;

	DWORD dwSize;

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp))
	{
		dwSize = 11;
		if (RegQueryValueEx(hKey, "ID", 0, NULL, (LPBYTE)AutoLogin.iUser, &dwSize) != ERROR_SUCCESS)
		{
			ZeroMemory(AutoLogin.iUser, sizeof(AutoLogin.iUser));
		}
		dwSize = 11;
		if (RegQueryValueEx(hKey, "PW", 0, NULL, (LPBYTE)AutoLogin.iPassword, &dwSize) != ERROR_SUCCESS)
		{
			ZeroMemory(AutoLogin.iPassword, sizeof(AutoLogin.iPassword));
		}
		dwSize = sizeof(int);
		if (RegQueryValueEx(hKey, "SavePass", 0, NULL, (LPBYTE)&AutoLogin.savePass, &dwSize) != ERROR_SUCCESS)
		{
			AutoLogin.savePass = false;
		}
	}

	SetCompleteHook(0xE8, 0x0040B635, &cAutoLogin::TextLogin);
	SetCompleteHook(0xE8, 0x0040B5A9, &cAutoLogin::CButtonAutoLogin);
	SetCompleteHook(0xE8, 0x0040B400, &cAutoLogin::DoRequestLogin);
	SetCompleteHook(0xE8, 0x0040B44F, &cAutoLogin::DoRequestLogin);
	SetCompleteHook(0xE8, 0x00483711, &cAutoLogin::DoLoginWinCreate);
}

void cAutoLogin::TextLogin(int X, int Y, LPCTSTR Text)
{
	float startX = (640.0 - getX(329)) / 2, startY = (480.0 - getY(245)) * 2.0 / 3.0;

	pDrawTextAL(pTextThisAL(), X, Y, Text, 0, 0, (PINT)1, 0);

	pDrawTextAL(pTextThisAL(), startX + getX(124), startY + getY(158), "Save password", getX(120), 0, (PINT)4, 0);
}

bool cAutoLogin::WriteString(const std::string& name, const std::string& value) {
	HKEY	hKey = NULL;
	DWORD	dwDisp;
	DWORD	dwSize = value.size();

	if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp))
		return false;

	RegSetValueEx(hKey, name.c_str(), 0L, REG_SZ, (CONST BYTE*)value.c_str(), dwSize);
	RegCloseKey(hKey);
	return true;
}

bool cAutoLogin::WriteDword(const std::string& name, DWORD value) // OK
{
	HKEY	hKey = NULL;
	DWORD	dwDisp;
	DWORD	dwSize = sizeof(DWORD);

	if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp))
		return false;

	RegSetValueEx(hKey, name.c_str(), 0L, REG_DWORD, (BYTE*)&value, dwSize);
	RegCloseKey(hKey);
	return true;
}

void cAutoLogin::DoRequestLogin() // OK
{
	int Struct = _Instance() + 16968;

	InputBox_GetText(*(DWORD*)(Struct + 848), AutoLogin.iUser, 11);
	InputBox_GetText(*(DWORD*)(Struct + 852), AutoLogin.iPassword, 11);


	if (AutoLogin.savePass) {
		WriteString("ID", AutoLogin.iUser);
		WriteString("PW", AutoLogin.iPassword);
		WriteDword("SavePass", 1);
	}
	else {
		WriteDword("SavePass", 0);
	}
	
	RequestLogin(_Instance() + 16968);

}

void cAutoLogin::CButtonAutoLogin(int This, int a2) // OK
{
	CUIRenderText_SetFont((int)pTextThis(), *(int*)0xE8C594);

	float startX = (640.0 - getX(329)) / 2;
	float startY = (480.0 - getY(245)) * 2.0 / 3.0;

	if (IsCursorInZone(startX + getX(246.0), startY + getY(156), 15, 15))
	{
		if (pIsKeyRelease(VK_LBUTTON))
		{
			RenderBitmap(0x7B69, startX + getX(246.0), startY + getY(156), 15 / g_fScreenRate_x, 15 / g_fScreenRate_y, 0.01, 0.02999999747, 0.5799999952, 0.3700000346, 1, 1, 0);
			AutoLogin.savePass = !AutoLogin.savePass;
			PlayBuffer(25, 0, 0);
		}
	}
	if (AutoLogin.savePass)
	{ 
		    RenderBitmap(0x7B69, startX + getX(246.0), startY + getY(156), 15 / g_fScreenRate_x, 15 / g_fScreenRate_y, 0.01, 0.02999999747, 0.8799999952, 0.3700000346, 1, 1, 0);
	}
	else
	{
		   RenderBitmap(0x7B69, startX + getX(246.0), startY + getY(156), 15 / g_fScreenRate_x, 15 / g_fScreenRate_y, 0.01, 0.49999999747, 0.8799999952, 0.3700000346, 1, 1, 0);
	}

}

void cAutoLogin::DoLoginWinCreate(int)
{
	int Struct = _Instance() + 16968;

	LoginWinCreate(Struct);

	if (AutoLogin.savePass) {

		InputBox_SetText(*(DWORD*)(Struct + 848), AutoLogin.iUser);

		InputBox_SetText(*(DWORD*)(Struct + 852), AutoLogin.iPassword);
	}
}