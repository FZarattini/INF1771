// Implementação do k-NN (k-nearest neighbors algorithm)

#include <iostream>
#include <vector>
#include <math.h>
#include <set>
#include <map>
//#include <string>
#include <string.h>
//#include <strings.h>

using namespace std;

class Flor
{
private:
	string classe;
	double arg1, arg2, arg3, arg4;

public:
	//Construtor
	Flor(double arg1, double arg2, double arg3, double arg4, string classe)
	{
		this->arg1 = arg1;
		this->arg2 = arg2;
		this->arg3 = arg3;
		this->arg4 = arg4;
		this->classe = classe;
	}

	//Getters
	double getArg1()
	{
		return arg1;
	}

	double getArg2()
	{
		return arg2;
	}

	double getArg3()
	{
		return arg3;
	}

	double getArg4()
	{
		return arg4;
	}
	
	string getClasse()
	{
		return classe;
	}
};

// função que retorna a distância euclidiana entre 2 indivíduos
double distancia(Flor ind1, Flor ind2)
{
	/*
		A distância euclidiana é a raiz quadrada da soma das
		diferenças dos valores dos atributos elevado ao quadrado
	*/
	
	double soma = pow((ind1.getArg1() - ind2.getArg1()), 2) + pow((ind1.getArg2() - ind2.getArg2()), 2) + pow((ind1.getArg3() - ind2.getArg3()), 2) + pow((ind1.getArg4() - ind2.getArg4()), 2);
	
	return sqrt(soma);
}

// essa função classifica uma nova amostra
string classifica(vector<Flor>& flores, Flor novo_exemplo, int K)
{
	// se o K for par decrementa
	//if(K % 2 == 0)
	//{
	//	K--;
	//	if(K <= 0)
	//		K = 1;
	//}
	
	if(K % 2 == 0 || K < 0){
		return "K deve ser um valor ímpar positivo!";
	}
		

	// obtém o tamanho do vetor
	int vetSize = flores.size();

	/*
		set de pairs da distância de cada indivíduo
		do conjunto de treinamento para o novo exemplo
		cada pair é composto pela distância e o índice
		do indivíduo no vetor
	*/
	set<pair<double, int> > dist_flores;

	/*
		calcula-se a distância euclidiana do novo exemplo
		para cada amostra do conjunto de treinamento
	*/
	for(int i = 0; i < vetSize; i++)
	{
		double dist = distancia(flores[i], novo_exemplo);
		dist_flores.insert(make_pair(dist, i));
	}
	/*
	para decidir a qual classe pertence o novo exemplo,
	basta verificar arg1 classe mais frequente dos K
	vizinhos mais próximos
	*/
	set<pair<double, int> >::iterator j;
	
	/*
		o contador de Iris-setosa estará no índice 0,
		o contador de Iris-versicolor estará no índice 1
		e o contador de Iris-virginica estará no índice 2
	*/
	vector<int> cont_classes(3);
	
	int contK = 0;

	for(j = dist_flores.begin(); j != dist_flores.end(); j++)
	{
		if(contK == K) break;
		
		string classe = flores[j->second].getClasse();

		if(classe == "Iris-setosa")
			cont_classes[0]++;
		else if(classe == "Iris-versicolor")
			cont_classes[1]++;
		else
			cont_classes[2]++;

		contK++;
	}
	
	string classe_classificacao;
	
	if(cont_classes[0] >= cont_classes[1] && cont_classes[0] >= cont_classes[2])
		classe_classificacao = "Iris-setosa";
	else if(cont_classes[1] >= cont_classes[0] && cont_classes[1] >= cont_classes[2])
		classe_classificacao = "Iris-versicolor";
	else
		classe_classificacao = "Iris-virginica";

	return classe_classificacao;
}


int main(int argc, char *argv[])
{

	vector<Flor> flores;

	/*
		o K é a quantidade de vizinhos que serão
		levados em conta para classificação de um
		novo dado, é recomendável que seja ímpar
		para que não possa haver empate
	*/
	int K = 3;
	
	// tamanho do conjunto de dados de treinamento
	int tam_treinamento = 105;

	/*
		o processo de treinamento consiste em apenas
		armazenar o conjunto de dados de treinamento
	*/
	for(int i = 0; i < tam_treinamento; i++)
	{
		string classe;
		double arg1, arg2, arg3, arg4;

		cin >> arg1 >> arg2 >> arg3 >> arg4 >> classe;

		flores.push_back(Flor(arg1, arg2, arg3, arg4, classe));
	}

	int acertos = 0;
	int tam_testes = 150 - tam_treinamento;
	
	// processo de classificação
	for(int i = 0; i < tam_testes; i++)
	{
		string classe;
		double arg1, arg2, arg3, arg4;

		cin >> arg1 >> arg2 >> arg3 >> arg4 >> classe;
		
		Flor ind(arg1, arg2, arg3, arg4, classe);
		
		string classe_obtida = classifica(flores, ind, K);
		
		cout << "Classe esperada: " << classe << "\n";
		cout << "Classe obtida: " << classe_obtida << "\n\n";
		
		if(classe == classe_obtida)
			acertos++;
	}
	
	cout << acertos << " acertos de um total de " << tam_testes << " testes.\n";

	return 0;
}
