#include "utility.h"

struct Points
{
	double x;
	double y;
};

struct ABvalues
{
	double a;
	double b;
};

int main()
{
	// Ler dados do arquivo
	ifstream dataFile;
	dataFile.open("pontos.txt");
	Points points[20];
	int C = 1, n = 0, i = 0, j = 0, k = 0;

	// Dê um aviso e saia se não conseguir ler o arquivo
	if (dataFile.fail())
	{
		cout << "Não foi possível encontrar o arquivo de entrada!";
		exit(1);
	}

	// Carregue os pontos na matriz
	double tempX, tempY;
	while (dataFile >> tempX >> tempY)
	{
		points[n].x = tempX;
		points[n].y = tempY;
		n++;
	}

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(4);

	// Calcule a tabela err(i,j)
	double errTable[20][20];	// for err(i,j)
	double a[20][20];			// for a(i,j)
	double b[20][26];			// for b(i,j)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			double tempErr = 0;
			if (i >= j)
			{
				tempErr = 0;
			}
			else
			{
				// Somatórios de X, Y, X^2 e XY para diferentes pares (i, j)
				double sumX = 0, sumY = 0, sumXX = 0, sumXY = 0;
				for (k = i; k <= j; k++)
				{
					sumX = sumX + points[k].x;
					sumY = sumY + points[k].y;
					sumXX = sumXX + (points[k].x * points[k].x);
					sumXY = sumXY + (points[k].x * points[k].y);
				}

				// Calcule a e b
				int N = j - i + 1;
				a[i][j] = (N * sumXY - sumX * sumY) / (N * sumXX - sumX * sumX);
				b[i][j] = (sumY - a[i][j] * sumX) / N;

				// Calcule o err(i,j)
				for (k = i; k <= j; k++)
				{
					tempErr = tempErr + (points[k].y - a[i][j] * points[k].x - b[i][j]) * (points[k].y - a[i][j] * points[k].x - b[i][j]);
				}
			}
			// Armazene valores err(i,j) na tabela
			errTable[i][j] = tempErr;
		}
	}

	double opt[21];
	int optIndex[21];
	opt[0] = 0;
	opt[1] = 0;
	optIndex[0] = -858993460;
	optIndex[1] = -858993460;
	for (j = 2; j < n + 1; j++)
	{
		double tempMin = INFINITY;
		int tempIndex = 0;
		for (i = 1; i < j; i++)
		{
			// Calcule OPT(j)
			double min = errTable[i - 1][j - 1] + C + opt[i];
			if (min < tempMin)
			{
				tempMin = min;
				tempIndex = i;
			}
		}

		opt[j] = tempMin;
		optIndex[j] = tempIndex;
	}

	cout.unsetf(ios::fixed);
	cout.unsetf(ios::showpoint);
	cout.precision(6);

	// Tabela OPT de saída
	cout << "j   OPT(j)   breakpoint(i)" << endl;
	for (i = 0; i < n + 1; i++)
	{
		cout << i << "   " << setw(7) << left << opt[i] << "   " << optIndex[i] << endl;
	}
	
	// Encontre os pontos de interrupção percorrendo a tabela OPT de trás para frente
	// e escolha os valores a, b correspondentes
	ABvalues FinalAB[20];
	int segIJpair[20][20];
	int segNum = 0;
	for (i = n + 1; i > 1; i--)
	{
		if (optIndex[i] > optIndex[i - 1])
		{
			FinalAB[segNum].a = a[optIndex[i]-1][n-1];
			FinalAB[segNum].b = b[optIndex[i]-1][n-1];
			segIJpair[segNum][0] = optIndex[i];
			segIJpair[segNum][1] = n;
			segNum++;
			n = optIndex[i];
		}
	}

	// Imprima as equações de linha segmentada
	for (i = 0; i < segNum; i++)
	{
		cout << "Linha para segmento " << segIJpair[i][0] << " a " << segIJpair[i][1] << " e y = " << FinalAB[i].a << "x + (" << FinalAB[i].b << ")" << endl;
	}
		
	return 0;
}
