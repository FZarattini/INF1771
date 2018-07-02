#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <string>
#include <fstream>

using namespace std;

//classe que caracteriza um ponto da amostra de dados
class Ponto
{
private:
	//atributos da classe
	int qtdAtributos, idPonto, idCluster;
	vector<double> atributos;
	string nome;

public:
	//construtor
	Ponto(int idPonto, vector<double>& atributos, string nome = "")
	{
		this->idPonto = idPonto;
		qtdAtributos = atributos.size();

		for(int i = 0; i < qtdAtributos; i++)
			this->atributos.push_back(atributos[i]);

		this->nome = nome;
		idCluster = -1;
	}
	
	//retorna o ID do ponto em questão
	int getID()
	{
		return idPonto;
	}
	
	//retorna a classe do ponto em questão
	string getNome()
	{
		return nome;
	}
	
	//retorna um atributo do ponto em questão a partir de seu índice
	double getAtributo(int index)
	{
		return atributos[index];
	}
	
	//retorna o ID do cluster ao qual o ponto pertence
	int getIdCluster()
	{
		return idCluster;
	}
	
	//retorna a quantidade de atributos do ponto
	int getQtdAtributos()
	{
		return qtdAtributos;
	}

	//determina o ID do cluster em qual o ponto está sendo inserido
	void setIdCluster(int idCluster)
	{
		this->idCluster = idCluster;
	}
};

//classe que caracteriza um cluster em qual pontos serao inseridos
class Cluster
{
private:
	//atributos da classe
	int idCluster;
	vector<double> centroides;
	vector<Ponto> pontos;

public:
	//construtor
	Cluster(int idCluster, Ponto ponto)
	{
		this->idCluster = idCluster;

		int qtdAtributos = ponto.getQtdAtributos();

		for(int i = 0; i < qtdAtributos; i++)
			centroides.push_back(ponto.getAtributo(i));

		pontos.push_back(ponto);
	}
	
	//retorna do ID do cluster em questao 
	int getID()
	{
		return idCluster;
	}
	
	//retorna um ponto inserido no cluster em questão a partir de seu índice
	Ponto getPonto(int index)
	{
		return pontos[index];
	}

	//retorna a quantidade de pontos inseridos no cluster em questao
	int getQtdPontos()
	{
		return pontos.size();
	}

	//retorna o centroide do cluster em questao a partir de seu indice
	double getCentroide(int index)
	{
		return centroides[index];
	}

	//insere um ponto no cluster em questao passado o ponto
	void inserePonto(Ponto ponto)
	{
		pontos.push_back(ponto);
	}
	
	//determina o atributo que servirá de centroide do cluster em questao
	void setCentroide(int index, double atributo)
	{
		centroides[index] = atributo;
	}

	//remove um ponto do cluster em questao passado o id do ponto
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

//classe que caracteriza o método Kmeans
class KMeans
{
public:
	//construtor
	KMeans(int K, int qtdPontos, int qtdAtributos, int maxIter)
	{
		this->K = K;
		this->qtdPontos = qtdPontos;
		this->qtdAtributos = qtdAtributos;
		this->maxIter = maxIter;
	}

	//processa os pontos da amostra e associa cada ponto ao centroide mais proximo
	void processa(vector<Ponto> & pontos)
	{		
		if(K > qtdPontos)
			return;
		
		//vetor de indices que devem ser ignorados no processo de criacao de centroides
		vector<int> ignorados;

		//escolhe K atributos diferentes para serem os centroides dos clusters
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

			//associa cada ponto ao centroide mais proximo
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

			//recalcula o centro de cada cluster a partir dos pontos que estao
			//inseridos
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
				break;
			}

			iter++;
		}

		//imprime os clusters, pontos inseridos neles e seus atributos na saida padrao
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

			cout << "Atributos do Cluster: ";

			for(int j = 0; j < qtdAtributos; j++)
				cout << clusters[i].getCentroide(j) << " ";

			cout << "\n\n";
		}
	}
	
	
	//le o arquivo de saida para verificar a porcentagem de pontos que foram
	//inseridos incorretamente em cada cluster
	void verifica(int K, int qtdPontos)
	{
		   vector<int> contadores;
		   ifstream inFile;
		   inFile.open("clusters.txt");
		   freopen("verificacao.txt","w",stdout);
		   string dummyLine;
		   string classe;
		   char ch;
		   int total, cluster = 0, flag = 0;
		   double erro, erroTotal = 0.0;
		   cout.precision(2);
		   
		   for(int i = 0;i < 3; i++)
		   {
			   contadores.push_back(0);   
		   }	   
		   
		   while(!inFile.eof()){
		   	   while(cluster < K){
			   	   while(inFile >> noskipws >> ch)
			   	   {					   		 	
	 	   		       if(ch == 'C'){
	 	   		       		if(flag != 0){
								cluster++;												   
							}
							flag = 1;
						   	getline(inFile, dummyLine);			   
					   }
	 	   		       	   	
				   	   else if(ch == 'I'){
	               	   	   for(int i = 0 ; i < 4; i++)
	               		 	 	 inFile >> noskipws >> ch;
	               		   getline(inFile, classe);    
	                   }
					   else if(ch == 'A')
					   {
						   total = contadores[0] + contadores[1] + contadores[2];
						      	
						   if((contadores[0] > contadores[1]) && (contadores[0] + contadores[2]))
							   erro = contadores[1] + contadores[2];
						    	 	  
						   else if((contadores[1] > contadores[0]) && (contadores[1] > contadores[2]))
							   erro = contadores[0] + contadores[2];
						   
						   else
						   	   erro = contadores[0] + contadores[1];	
						   
			               erroTotal += (erro / qtdPontos) * 100;
			               erro = (erro / total) * 100;
			               cout << "Porcentagem de pontos incorretos no cluster: " << cluster + 1 << "\n" << erro << "%" << "\n";
			               
			               erro = 0;
						   contadores[0] = 0;
						   contadores[1] = 0;
						   contadores[2] = 0;	  	
						   for(int i = 0 ; i < 2; i++)
						   	   getline(inFile, dummyLine);	    
					   }
					   
					   else if(ch == 'T'){
						   cout << "Porcentagem total de pontos agrupados incorretamente: " << erroTotal << "%\n";
						   return;
					   }
	 			   	   	   
					   
					   if(!classe.empty()){
						   if(classe == "versicolor")
						   {
						   		contadores[0]++;
						   		classe = '\0';
							   
						   }
						   else if(classe == "virginica")
						   {
							   contadores[1]++;
							   classe = '\0';   
						   }
						   else if(classe == "setosa")
						   {
							   contadores[2]++;
							   classe = '\0';
						   }	    	
					   }	  	
			   	   }
				}   
		   }
		   
		
		
	}	
	
private:
	vector<Cluster> clusters;
	int maxIter, qtdAtributos, qtdPontos, K;

	//retorna o id do centroide mais proximo ao ponto passado por parâmetro 
	//utilizando distância euclidianda
	int getIdMaisPerto(Ponto ponto)
	{
		int clusterCentro = 0;
		double soma = 0.0, distMinima;
		
		//calculo da distancia euclidiana minima entre os atributos do ponto e o 
		//centroide do primeiro cluster
		for(int i = 0; i < qtdAtributos; i++)
		{
			soma += pow(clusters[0].getCentroide(i) -
					   ponto.getAtributo(i), 2.0);
		}

		distMinima = sqrt(soma);

		//calcula a distância minima entre os atributos e os centroides dos
		//demais clusters. Caso encontre uma distancia menor, atualiza 
		//clusterCentro.
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
		
		//retorna o centroide mais proximo do ponto
		return clusterCentro;
	}
};

int main(int argc, char *argv[])
{
	srand (time(NULL));
	int K, qtdAtributos, qtdPontos, nomeado, maxIter;
	
	freopen("clusters.txt","w",stdout);
	
	K = atoi(argv[1]);
	cin >> qtdPontos >> qtdAtributos >> maxIter >> nomeado;

	vector<Ponto> pontos;
	string nomePonto;    

	//popula os vetores de pontos e de atributos
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

	//constroi a classe Kmeans
	KMeans kmeans(K, qtdPontos, qtdAtributos, maxIter);

	clock_t begin = clock();

	//processa os pontos da base de dados
	kmeans.processa(pontos);
	
	clock_t end = clock();
    
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    cout << "Tempo passado para o processo de agrupamento: " << elapsed_secs << "\n";
    
    begin = clock();
    
    //verifica os erros nos agrupamentos
    kmeans.verifica(K, qtdPontos);
    
	end = clock();
    
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Tempo passado para o processo de verificacao: " << elapsed_secs << "\n";

	return 0;
}
