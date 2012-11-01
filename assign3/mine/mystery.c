int mystery(int n)
{

	int i = 0;

	int var1 = 1;
	int var2 = 0;
	int var3 = 0;

	while (i <= n)
	{
		var3 = var1 + var2;
		var2 = var1;
		var1 = var3;
		i++;
	}

	return var1;

}
