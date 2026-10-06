#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <ctime>

using namespace std;

FILE *ArqProduto; // arquivo 

typedef struct data {
	int dia, mes, ano;
	int hora, min, seg;
} DATA;

typedef struct Produto {
	char DescricaoProd[50];
	long int Codigo;
	int Qtd;
	DATA DataCadastro;        // data e hora do cadastro
	float PrecoCusto, PrecoVenda;
} RegProduto;

// Variavel Registro
RegProduto Prod;

// Protótipos
void MenuProdutosBar();
void pausa();
void Cadastrar();
DATA gerarData();
void imprimirData(DATA D);
long int ultimoCod();
void Relatorio();
void BuscaProduto();
long int ConsultaProduto(int cod);
void MenuEdicao();
void EditarProduto(int pos);
void MenuExclusao();
long int ExcluirProduto(int cod);

int main(int argc, char** argv) {
	setlocale(LC_ALL, "Portuguese");
	MenuProdutosBar();
	return 0;
}

void MenuProdutosBar() {
	int opcao;
	do {
		system("cls");
		cout << "\n ================ SISTEMA DE CONTROLE DE PRODUTOS ================";
		cout << "\n\n 		    ------- Usuário: Bar do Tony & JP -------";
		cout << "\n =================================================================";
		cout << "\n 1 - Cadastrar Produto";
		cout << "\n 2 - Relatório dos Produtos";
		cout << "\n 3 - Pesquisar Produto";
		cout << "\n 4 - Editar Produto";
		cout << "\n 5 - Excluir Produto";
		cout << "\n 0 - Encerrar Programa";
		cout << "\n\n - Escolha a opção: ";
		cin >> opcao;
		switch(opcao) {
			case 1: Cadastrar();
					break;
			case 2: Relatorio();
					pausa();
					break;
			case 3: system("cls");
					BuscaProduto();
					pausa();
					break;	
			case 4: MenuEdicao();
					break;
			case 5: MenuExclusao();
					break;
			default: cout << "\n Tecle <Enter> para sair do programa "; system("pause>>null");
					break;
		}
	} while(opcao != 0);
}

void pausa() {
	cout << "\n Tecle <Enter> para voltar ao Menu de Opções";
	system("pause>>null");
}

long int ultimoCod() {
	long int cod;
	RegProduto aux;                             // local: não mexe na global Prod
	FILE *f = fopen("Produtos.dat", "rb");      // local: não mexe na global ArqProduto

	if(f == NULL) return 1;

	if(fread(&aux, sizeof(aux), 1, f) == 0) {   // arquivo vazio: 1º código
		cod = 1;
	} else {
		fseek(f, -(long)sizeof(aux), SEEK_END); // vai ao início do último registro
		fread(&aux, sizeof(aux), 1, f);         // lê o último registro
		cod = aux.Codigo + 1;
	}

	fclose(f);                                  // fecha o arquivo
	return cod;
}

void Cadastrar() {
	char opcao;
	system("cls");
	// Cria um arquivo para .dat para armazenar os produtos
	ArqProduto = fopen("Produtos.dat", "ab");
	if(ArqProduto == NULL) {
		cout << "Erro na abertura do arquivo \n";
		system("pause");
		exit(1);
	}
	
	do {
		Prod.Codigo = ultimoCod();
		cout << "\nCódigo:  " << Prod.Codigo;
		cin.ignore(80, '\n');
		cout << "\nDescrição:  ";
		cin.getline(Prod.DescricaoProd, sizeof(Prod.DescricaoProd)); 
		cout << "Quantidade Estoque (por unidade): ";
		cin >> Prod.Qtd;
		while(Prod.Qtd < 0) {
			cout << "\nQuantidade Inválida! Digite novamente: ";
			cin >> Prod.Qtd;
		}
		
		cout << "Preço de Custo (R$ 0.00): ";
		cin >> Prod.PrecoCusto;
		while(Prod.PrecoCusto < 0) {
			cout << "\nPreço de Custo Inválida! Digite novamente: ";
			cin >> Prod.PrecoCusto;
		}
		cout << "Preço de Venda (R$ 0.00): ";
		cin >> Prod.PrecoVenda;
		while(Prod.PrecoVenda < 0) {
			cout << "\nPreço de Venda Inválida! Digite novamente: ";
			cin >> Prod.PrecoVenda;
		}
		
		Prod.DataCadastro = gerarData();
		cout << "Data e Hora do cadastro: ";
		imprimirData(Prod.DataCadastro);
		cout << endl;
		
		fwrite(&Prod, sizeof(Prod), 1, ArqProduto); // Gravando no arquivo
		fflush(ArqProduto);   // grava o buffer no disco agora, para o ultimoCod() ver esse registro
		cout << "\n Adicionar outro produto (S/N)? ";
		cin >> opcao;
	} while(toupper(opcao) == 'S');
	fclose(ArqProduto); // Fechando Arquivo
}

DATA gerarData() {
	DATA D;
	time_t mytime = time(NULL);
	struct tm tm = *localtime(&mytime);
	D.dia  = tm.tm_mday;
	D.mes  = tm.tm_mon + 1;
	D.ano  = tm.tm_year + 1900;
	D.hora = tm.tm_hour;
	D.min  = tm.tm_min;
	D.seg  = tm.tm_sec;
	return D;
}

// Imprime no formato dd/mm/aaaa hh:mm:ss (19 caracteres)
void imprimirData(DATA D) {
	if(D.dia < 10)
		cout << "0" << D.dia << "/";
	else 
		cout << D.dia << "/";
	if(D.mes < 10)
		cout << "0" << D.mes << "/";
	else 
		cout << D.mes << "/";
	cout << D.ano << " ";
	if(D.hora < 10)
		cout << "0" << D.hora << ":";
	else 
		cout << D.hora << ":";
	if(D.min < 10)
		cout << "0" << D.min << ":";
	else 
		cout << D.min << ":";
	if(D.seg < 10)
		cout << "0" << D.seg;
	else
		cout << D.seg;
}	

void Relatorio() {
	system("cls");
	ArqProduto = fopen("Produtos.dat", "rb"); // open read binary
	if(ArqProduto == NULL) {
		cout << "Nenhum produto cadastrado ainda \n";
		pausa();
		return;
	}
	cout << "\n                                *** PRODUTOS CADASTRADOS ***";
	cout << "\n ---------------------------------------------------------------------------------------------\n";
	cout << setw(8) << "Código";		cout << setw(20) << "Descrição";
	cout << setw(20) << "Quantidade Estoque"; 	cout << setw(14) << "Preço Custo";
	cout << setw(14) << "Preço Venda";	cout << setw(21) << "Data e Hora";
	cout << "\n ----------------------------------------------------------------------------------------------";
	
	while(fread(&Prod, sizeof(Prod), 1, ArqProduto) == 1) {
		cout << "\n";
		cout << setw(8) << Prod.Codigo; 
		cout << setw(20) << Prod.DescricaoProd;
		cout << setw(20) << Prod.Qtd;
		cout << setw(14) << Prod.PrecoCusto;
		cout << setw(14) << Prod.PrecoVenda;
		cout << "  ";                       // 2 espaços + 19 da data = 21 (alinha com o cabeçalho)
		imprimirData(Prod.DataCadastro);
	}
	cout << endl;
		
	fclose(ArqProduto);
}

void BuscaProduto() {
	int buscaCod, result;
	system("cls");
	cout << "\n *** CONSULTAR PRODUTO ***";
	cout << "\n ---------------------------------------\n";
	cout << "\n Digite o código do produto: ";
	cin >> buscaCod;
	result = ConsultaProduto(buscaCod);
	if(result == -1) {
		cout << "\n Produto não cadastrado";
	} else {
		ArqProduto = fopen("Produtos.dat", "rb");
		fseek(ArqProduto, result, SEEK_SET);
		fread(&Prod, sizeof(Prod), 1, ArqProduto);
		fclose(ArqProduto);
		cout << "\n --------------------------------------";
		cout << "\n  *** INFORMAÇÕES DO PRODUTO ***";
		cout << "\n --------------------------------------";
		cout << "\n  Código : " << Prod.Codigo;
		cout << "\n  Descrição : " << Prod.DescricaoProd;
		cout << "\n  Quantidade em Estoque : " << Prod.Qtd;
		cout << "\n  Preço de Custo : " << Prod.PrecoCusto;
		cout << "\n  Preço de Venda : " << Prod.PrecoVenda;
		cout << "\n  Data  e  Horas : "; imprimirData(Prod.DataCadastro);
		cout << "\n --------------------------------------";
	}
}

long int ConsultaProduto(int cod) {
	system("cls");
	ArqProduto = fopen("Produtos.dat", "rb");
	if(ArqProduto == NULL) {
		cout << "Erro na abertura do arquivo \n";
		pausa();
		exit(1);
	}		
	while(fread(&Prod, sizeof(Prod), 1, ArqProduto) == 1) {
		if(Prod.Codigo == cod) {
			long int posicao = ftell(ArqProduto) - (long)sizeof(Prod); // posição do registro
			fclose(ArqProduto);
			return posicao;
		}
	}
	fclose(ArqProduto);
	return -1;
}

void MenuEdicao() {
	int cod, pos;
	int opcaoM;
	system("cls");
	do {
		cout << "\n -----------------------------------------\n";
		cout << "           *** MENU DE MODIFICAÇÃO ***";
		cout << "\n -----------------------------------------\n";
		cout << "\n1) Ver Relatório de Produtos Cadastrados; ";
		cout << "\n0) Para retornar ao Menu Principal;       ";
		cout << "\n- Digite sua escolha: ";
		cin >> opcaoM;
		while((opcaoM < 0) || (opcaoM > 1)) {
			cout << "\n Opção Inválida! Digite novamente: ";
			cin >> opcaoM;
		} 
		if(opcaoM == 1) {
			Relatorio();
			cout << "\n Se quiser retorna a página anterior (Digite = 0) ";
			cout << "\n- Código do Produto que será modificado: ";
			cin >> cod;
			if(cod == 0) {
				pausa();
				system("cls");
			} else {
				pos = ConsultaProduto(cod);
				if(pos == -1) {
					cout << "\n Produto não existe";
					pausa();
				} else {
					system("cls");
					ArqProduto = fopen("Produtos.dat", "rb");
					fseek(ArqProduto, pos, SEEK_SET);
					fread(&Prod, sizeof(Prod), 1, ArqProduto);
					fclose(ArqProduto);
					cout << "\n --------------------------------------";
					cout << "\n  *** INFORMAÇÕES DO PRODUTO ***";
					cout << "\n --------------------------------------";
					cout << "\n  Código : " << Prod.Codigo;
					cout << "\n  Descrição : " << Prod.DescricaoProd;
					cout << "\n  Data  e  Horas : "; imprimirData(Prod.DataCadastro);
	 				cout << "\n  Quantidade em Estoque : " << Prod.Qtd;
					cout << "\n  Preço de Custo : " << Prod.PrecoCusto;
					cout << "\n  Preço de Venda : " << Prod.PrecoVenda;
					cout << "\n --------------------------------------";
					EditarProduto(pos);
				}	
			}
		}
	} while(opcaoM != 0);
	pausa();
} 

void EditarProduto(int pos) {
	char escolha;
	ArqProduto = fopen("Produtos.dat", "r+b");
	fseek(ArqProduto, pos, SEEK_SET);
	fread(&Prod, sizeof(Prod), 1, ArqProduto);
	
	cout << "\n *** MODIFICAR PRODUTO *** ";
	cout << "\n- Modificar Descrição (S/N): ";
	cin >> escolha;
	if(toupper(escolha) == 'S') {
		cout << "\n- Nova Descrição: ";
		cin.ignore(80, '\n');
		cin.getline(Prod.DescricaoProd, sizeof(Prod.DescricaoProd)); 
	}
	cout << "\n- Modificar a Quantidade em Estoque (S/N): ";
	cin >> escolha;
	if(toupper(escolha) == 'S') {
		cout << "\n- Nova Quantidade em Estoque (por unidade): ";
		cin >> Prod.Qtd;
		while(Prod.Qtd < 0) {
			cout << "\nQuantidade Inválida! Digite novamente: ";
			cin >> Prod.Qtd;
		}
	}
	
	cout << "\n- Modificar Preço de Custo (S/N): ";
	cin >> escolha;
	if(toupper(escolha) == 'S') {
		cout << "\n- Nova Preço de Custo (R$ 0.00): ";
		cin >> Prod.PrecoCusto; 
		while(Prod.PrecoCusto < 0) {
			cout << "\n Preço de Custo Inválido! Digite novamente: ";
			cin >> Prod.PrecoCusto;
		}
	}
	
	cout << "\n- Modificar Preço de Venda (S/N): ";
	cin >> escolha;
	if(toupper(escolha) == 'S') {
		cout << "\n- Nova Venda de Venda (R$ 0.00): ";
		cin >> Prod.PrecoVenda; 
		while(Prod.PrecoVenda < 0) {
			cout << "\n Preço de Venda Inválido! Digite novamente: ";
			cin >> Prod.PrecoVenda;
		}
	}
	fseek(ArqProduto, pos, SEEK_SET); // volta ao início do registro
	fwrite(&Prod, sizeof(Prod), 1, ArqProduto);
	fclose(ArqProduto);
	cout << "\n Produto Alterado com sucesso";
	pausa();
	system("cls");
}

void MenuExclusao() {
	system("cls");
	int opcaoE, posicao, cod;
	char excluir;
	do {
		cout << "\n -----------------------------------------\n";
		cout << "           *** MENU DE EXCLUSÃO ***";
		cout << "\n -----------------------------------------\n";
		cout << "\n1) Ver Relatório de Produtos Cadastrados; ";
		cout << "\n0) Para retornar ao Menu Principal;       ";
		cout << "\n- Digite sua escolha: ";
		cin >> opcaoE;
		while((opcaoE < 0) || (opcaoE > 1)) {
			cout << "\nEscolha inválida! Digite Novamente";
			cin >> opcaoE;
		}
		if(opcaoE == 1) {
			Relatorio();
			cout << "\n- Pesquise qual Item quer excluir: ";
			cin >> cod;
			posicao = ConsultaProduto(cod);
			if (posicao == -1) {
				cout << "\n Item não cadastrado";
				pausa();
			} else {
				system("cls");
				ArqProduto = fopen("Produtos.dat", "rb");
				fseek(ArqProduto, posicao, SEEK_SET);
				fread(&Prod, sizeof(Prod), 1, ArqProduto);
				fclose(ArqProduto);
				cout << "\n --------------------------------------";
				cout << "\n  *** INFORMAÇÕES DO PRODUTO ***";
				cout << "\n --------------------------------------";
				cout << "\n  Código : " << Prod.Codigo;
				cout << "\n  Descrição : " << Prod.DescricaoProd;
	 			cout << "\n  Quantidade em Estoque : " << Prod.Qtd;
				cout << "\n  Preço de Custo : " << Prod.PrecoCusto;
				cout << "\n  Preço de Venda : " << Prod.PrecoVenda;
				cout << "\n  Data  e  Horas : "; imprimirData(Prod.DataCadastro);
				cout << "\n --------------------------------------";
				cout << "\n Excluir (S/N)?";
				cin >> excluir;
				if(toupper(excluir) == 'S') {
					ExcluirProduto(cod);
					cout << "Exclusão feita com sucesso";
				} else {
					cout << "Exclusão cancelada";
				}
				pausa();
				system("cls");
			}
		} 
	} while(opcaoE != 0);
	pausa();
}

long int ExcluirProduto(int cod) {
	system("cls");
	long int pos = -1; // Posicao do registro excluído
	FILE *ArqTemp;
	
	ArqProduto = fopen("Produtos.dat", "rb");
	if(ArqProduto == NULL) {
		cout << "Erro na abertura do arquivo \n";
		pausa();
		exit(1);
	}
			
	ArqTemp = fopen("Temp.dat", "wb");
	if(ArqTemp == NULL) {
		cout << "Erro ao criar arquivo temporario \n";
		fclose(ArqProduto);
		pausa();
		exit(1);
	}
	
	while(fread(&Prod, sizeof(Prod), 1, ArqProduto) == 1) {
		if(Prod.Codigo == cod) {
			pos = ftell(ArqProduto) - (long)sizeof(Prod); // guarda posicao, nao copia o registro
		} else { 
			fwrite(&Prod, sizeof(Prod), 1, ArqTemp); // copia os demais
		}
	}
	fclose(ArqProduto);
	fclose(ArqTemp);
	
	if(pos != -1) {							// ArqTemp se torna o ArqProduto
		remove("Produtos.dat");				
		rename("Temp.dat", "Produtos.dat");
	} else {
		remove("Temp.dat");
	}
	return pos;
}

