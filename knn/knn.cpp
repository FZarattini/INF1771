#include <math.h>
#include <map>
#include <ctime>
#include <iostream>
#include <vector>
#include <set>
#include<cstdlib>

using namespace std;

//criando a classe for
class Flor
{
	//variaveis
private:
	string classe;
	double arg1, arg2, arg3, arg4; //parametros das flores + tipo

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

// função encarregada de retornar a distancia euclidiana entre 2 pontos (individuos)
double distancia(Flor flor1, Flor flor2)
{
	//distancia euclidiana
	
	double soma = pow((flor1.getArg1() - flor2.getArg1()), 2) + pow((flor1.getArg2() - flor2.getArg2()), 2) + pow((flor1.getArg3() - flor2.getArg3()), 2) + pow((flor1.getArg4() - flor2.getArg4()), 2);
	
	return sqrt(soma);
}

// classifica nova amostra
string classifica(vector<Flor>& flores, Flor novo_exemplo, int K)
{
	// obtem o tamanho do vetor de Flor passano como parametro na funçao
	int vetSize = flores.size();

	//contador de votos
	int contK = 0;
	

	//cria um par composto de:
	//distância e o índice do indivíduo no vetor
	//a distancia eh a distancia de cada individuo do conjunto de treinamento para o novo exemplo.

	set<pair<double, int> > dist_flores;

	//for que calcula a distancia euclidiana do novo exemplo pra cada amostra do conjunto de treinamento
	for(int i = 0; i < vetSize; i++)
	{
		double dist = distancia(flores[i], novo_exemplo);
		
		//insere novo par
		dist_flores.insert(make_pair(dist, i));
	}
	
	
	//o novo exemplo será classificado dependendo de quem possuir maioria nos K primeiros votos
	//que estão ordenados do mais próximo para o mais distante
	
	set<pair<double, int> >::iterator j;
	
	
	//votos de Iris-setosa sao guardados no indice 0
	//votos de Iris-versicolor sao guardados no indice 1
	//votos de Iris-virginica sao guardados no indice 2
	vector<int> guarda_votos(3);
	
	//for do inicio de dist_flores até o final
	for(j = dist_flores.begin(); j != dist_flores.end(); j++)
	{	
		//quando atingirmos o total de votos, pare
		if(contK == K)
		{
			break;
		}
		
		//pega a classe do vizinho mais próximo
		string classe = flores[j->second].getClasse();

		//guarda os votos
		if(classe == "Iris-setosa")
			guarda_votos[0]++;
		else if(classe == "Iris-versicolor")
			guarda_votos[1]++;
		else
			guarda_votos[2]++;
		
		//passa pro proximo voto
		contK++;
	}
	
	//preparamos o return da classe obtida
	string classificacao_obtida;
	
	//verificacoes para ver qual classe foi obtida
	if(guarda_votos[0] >= guarda_votos[1] && guarda_votos[0] >= guarda_votos[2])
		classificacao_obtida = "Iris-setosa";
	else if(guarda_votos[1] >= guarda_votos[0] && guarda_votos[1] >= guarda_votos[2])
		classificacao_obtida = "Iris-versicolor";
	else
		classificacao_obtida = "Iris-virginica";

	return classificacao_obtida;
}

int main(int argc, char *argv[])
{
    vector<Flor> flores;

    //K vizinhos que serao observados para classificar uma nova amostra por input do usuario
    int K = atoi(argv[1]);
	
    //tamanho do conjunto de dados de treinamento por input do usuario
	int tam_treinamento = atoi(argv[2]);

     if(K % 2 == 1 || K < 0){    	

    //o conjunto de treinamento fica guardado para ser consultado
    clock_t begin = clock();
    
    for(int i = 0; i < tam_treinamento; i++)
    {
        string classe;
        double arg1, arg2, arg3, arg4;

        //le os parametros no arquivo e armazena eles nas variaveis

        cin >> arg1 >> arg2 >> arg3 >> arg4 >> classe;

        flores.push_back(Flor(arg1, arg2, arg3, arg4, classe));
    }
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    cout << "Tempo passado para calcular o conjunto de treinamento: " << elapsed_secs << "\n";

    int acertos = 0;
    int tam_testes = 150 - tam_treinamento;

    // processo de classificacao
    begin = clock();
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
    
	end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    cout << "Tempo passado para calcular o processo de classificacao: " << elapsed_secs << "\n";

    cout << acertos << " acertos de um total de " << tam_testes << " testes.\n";
	cout << "A porcentagem de classificacoes corretas foi de: " << (acertos*100/tam_testes) << "%\n";
}else{
cout << "K deve ser impar positivo!";
}

    return 0;
}
