#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;

class Ponto
{
private:
	int qtdAtributos, idPonto, idCluster;
	vector<double> atributos;
	string nome;

public:
	Ponto(int idPonto, vector<double>& atributos, string nome = "")
	{
		this->idPonto = idPonto;
		qtdAtributos = atributos.size();

		for(int i = 0; i < qtdAtributos; i++)
			this->atributos.push_back(atributos[i]);

		this->nome = nome;
		idCluster = -1;
	}

	int getID()
	{
		return idPonto;
	}
	
	string getNome()
	{
		return nome;
	}
	
	double getAtributo(int index)
	{
		return atributos[index];
	}
	
	int getIdCluster()
	{
		return idCluster;
	}
	
	int getQtdAtributos()
	{
		return qtdAtributos;
	}

	void setIdCluster(int idCluster)
	{
		this->idCluster = idCluster;
	}
};

class Cluster
{
private:
	int idCluster;
	vector<double> centroides;
	vector<Ponto> pontos;

public:
	Cluster(int idCluster, Ponto ponto)
	{
		this->idCluster = idCluster;

		int qtdAtributos = ponto.getQtdAtributos();

		for(int i = 0; i < qtdAtributos; i++)
			centroides.push_back(ponto.getAtributo(i));

		pontos.push_back(ponto);
	}
	
	int getID()
	{
		return idCluster;
	}
	
	Ponto getPonto(int index)
	{
		return pontos[index];
	}

	int getQtdPontos()
	{
		return pontos.size();
	}

	double getCentroide(int index)
	{
		return centroides[index];
	}

	void inserePonto(Ponto ponto)
	{
		pontos.push_back(ponto);
	}
	
	void setCentroide(int index, double atributo)
	{
		centroides[index] = atributo;
	}

	bool removePonto(int idPonto)
	{
		int qtdPontos = pontos.size();

		for(int i = 0; i < qtdPontos; i++)
		{
			if(pontos[i].getID() == idPonto)
			{
				pontos.erase(pontos.begin() + i);
				return true;
			}
		}
		return false;
	}
};

class KMeans
{
private:
	vector<Cluster> clusters;
	int maxIter, qtdAtributos, qtdPontos, K;

	// return ID of nearest center (uses euclidean distance)
	int getIdMaisPerto(Ponto ponto)
	{
		int clusterCentro = 0;
		double soma = 0.0, distMinima;

		for(int i = 0; i < qtdAtributos; i++)
		{
			soma += pow(clusters[0].getCentroide(i) -
					   ponto.getAtributo(i), 2.0);
		}

		distMinima = sqrt(soma);

		for(int i = 1; i < K; i++)
		{
			double distancia;
			soma = 0.0;

			for(int j = 0; j < qtdAtributos; j++)
			{
				soma += pow(clusters[i].getCentroide(j) -
						   ponto.getAtributo(j), 2.0);
			}

			distancia = sqrt(soma);

			if(distancia < distMinima)
			{
				distMinima = distancia;
				clusterCentro = i;
			}
		}

		return clusterCentro;
	}

public:
	KMeans(int K, int qtdPontos, int qtdAtributos, int maxIter)
	{
		this->K = K;
		this->qtdPontos = qtdPontos;
		this->qtdAtributos = qtdAtributos;
		this->maxIter = maxIter;
	}

	void processa(vector<Ponto> & pontos)
	{
		if(K > qtdPontos)
			return;

		vector<int> ignorados;

		// choose K distinct atributos for the centers of the clusters
		for(int i = 0; i < K; i++)
		{
			while(true)
			{
				int index_point = rand() % qtdPontos;

				if(find(ignorados.begin(), ignorados.end(),
						index_point) == ignorados.end())
				{
					ignorados.push_back(index_point);
					pontos[index_point].setIdCluster(i);
					Cluster cluster(i, pontos[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}

		int iter = 1;

		while(true)
		{
			bool terminou = true;

			// associates each ponto to the nearest center
			for(int i = 0; i < qtdPontos; i++)
			{
				int centroMaisProximo = getIdMaisPerto(pontos[i]);
				int clusterAntigo = pontos[i].getIdCluster();
				
				if(clusterAntigo != centroMaisProximo)
				{
					if(clusterAntigo != -1)
						clusters[clusterAntigo].removePonto(pontos[i].getID());

					pontos[i].setIdCluster(centroMaisProximo);
					clusters[centroMaisProximo].inserePonto(pontos[i]);
					terminou = false;
				}
			}

			// recalculating the center of each cluster
			for(int i = 0; i < K; i++)
			{
				for(int j = 0; j < qtdAtributos; j++)
				{
					int qtdPontosNoCluster = clusters[i].getQtdPontos();
					double soma = 0.0;

					if(qtdPontosNoCluster > 0)
					{
						for(int p = 0; p < qtdPontosNoCluster; p++)
							soma += clusters[i].getPonto(p).getAtributo(j);
						clusters[i].setCentroide(j, soma / qtdPontosNoCluster);
					}
				}
			}

			if(terminou == true || iter >= maxIter)
			{
				cout << "Terminou na itera��o " << iter << "\n\n";
				break;
			}

			iter++;
		}

		// shows elements of clusters
		for(int i = 0; i < K; i++)
		{
			int qtdPontosNoCluster =  clusters[i].getQtdPontos();

			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for(int j = 0; j < qtdPontosNoCluster; j++)
			{
				cout << "Ponto " << clusters[i].getPonto(j).getID() + 1 << ": ";
				for(int p = 0; p < qtdAtributos; p++)
					cout << clusters[i].getPonto(j).getAtributo(p) << " ";

				string nomePonto = clusters[i].getPonto(j).getNome();

				if(nomePonto != "")
					cout << "- " << nomePonto;

				cout << endl;
			}

			cout << "Cluster atributos: ";

			for(int j = 0; j < qtdAtributos; j++)
				cout << clusters[i].getCentroide(j) << " ";

			cout << "\n\n";
		}
	}
};

int main(int argc, char *argv[])
{
	srand (time(NULL));

	int K, qtdAtributos, qtdPontos, nomeado, maxIter;

	cin >> qtdPontos >> qtdAtributos >> K >> maxIter >> nomeado;

	vector<Ponto> pontos;
	string nomePonto;

	for(int i = 0; i < qtdPontos; i++)
	{
		vector<double> atributos;

		for(int j = 0; j < qtdAtributos; j++)
		{
			double atributo;
			cin >> atributo;
			atributos.push_back(atributo);
		}

		if(nomeado)
		{
			cin >> nomePonto;
			Ponto p(i, atributos, nomePonto);
			pontos.push_back(p);
		}
		else
		{
			Ponto p(i, atributos);
			pontos.push_back(p);
		}
	}

	KMeans kmeans(K, qtdPontos, qtdAtributos, maxIter);
	kmeans.processa(pontos);

	return 0;
}