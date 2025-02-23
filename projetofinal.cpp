#include <iostream>
#include <string>
#include <list>
#include <cmath>

using namespace std;

struct Significado {
    string descricao;

    Significado(const string &desc) : descricao(desc) {}
};

struct Palavra {
    string nome;
    int x, y, z;
    list<Significado*> significados;

    Palavra(const string &n, int xCoord, int yCoord, int zCoord)
        : nome(n), x(xCoord), y(yCoord), z(zCoord) {}
};

struct NoArvore {
    Palavra* palavra;
    NoArvore* esquerda;
    NoArvore* direita;

    NoArvore(Palavra* p) : palavra(p), esquerda(NULL), direita(NULL) {}
};

struct ArvoreAlfabetica {
    NoArvore* raiz;

    ArvoreAlfabetica() : raiz(NULL) {}

    void inserir(NoArvore*& no, Palavra* palavra) {
        if (!no) {
            no = new NoArvore(palavra);
            return;
        }

        if (palavra->nome <= no->palavra->nome) {
            inserir(no->esquerda, palavra);
        } else {
            inserir(no->direita, palavra);
        }
    }

    void inserir(Palavra* palavra) {
        inserir(raiz, palavra);
    }

    void exibirEmOrdem(NoArvore* no) {
        if (!no) return;

        exibirEmOrdem(no->esquerda);
        cout << no->palavra->nome << endl;
        exibirEmOrdem(no->direita);
    }

    void exibir() {
        cout << "\nPalavras em ordem alfabética:\n";
        exibirEmOrdem(raiz);
    }
};

struct ArvorePorTamanho {
    NoArvore* raiz;

    ArvorePorTamanho() : raiz(NULL) {}

    void inserir(NoArvore*& no, Palavra* palavra) {
        if (!no) {
            no = new NoArvore(palavra);
            return;
        }

        if (palavra->nome.size() >= no->palavra->nome.size()) {
            inserir(no->esquerda, palavra);
        } else {
            inserir(no->direita, palavra);
        }
    }

    void inserir(Palavra* palavra) {
        inserir(raiz, palavra);
    }

    void exibirEmOrdem(NoArvore* no) {
        if (!no) return;

        exibirEmOrdem(no->esquerda);
        cout << no->palavra->nome << " (" << no->palavra->nome.size() << " letras)" << endl;
        exibirEmOrdem(no->direita);
    }

    void exibir() {
        cout << "\nPalavras ordenadas por tamanho (decrescente):\n";
        exibirEmOrdem(raiz);
    }
};

class GrafoDePalavras {
private:
    list<Palavra*> palavras;

public:
    bool palavraExiste(const string &nome) {
        for (list<Palavra*>::iterator it = palavras.begin(); it != palavras.end(); ++it) {
            if ((*it)->nome == nome) {
                return true;
            }
        }
        return false;
    }

    void inserirPalavra(const string &nome, int x, int y, int z, ArvoreAlfabetica &arvoreAlfabetica, ArvorePorTamanho &arvorePorTamanho) {
        if (palavraExiste(nome)) {
            cout << "Palavra já existe no grafo.\n";
            return;
        }

        arvoreAlfabetica.inserir(new Palavra(nome, x, y, z));
        arvorePorTamanho.inserir(new Palavra(nome, x, y, z));
        Palavra* novaPalavra = new Palavra(nome, x, y, z);
        string significadoDesc;

        cout << "Digite os significados para a palavra (digite '-1' para parar):\n";
        cin.ignore();
        while (true) {
            getline(cin, significadoDesc);
            if (significadoDesc == "-1") break;

            Significado* novoSignificado = new Significado(significadoDesc);
            novaPalavra->significados.push_back(novoSignificado);
        }

        palavras.push_back(novaPalavra);
    }

    void conectarPalavrasPorProximidade(int distanciaMaxima) {
        for (list<Palavra*>::iterator it1 = palavras.begin(); it1 != palavras.end(); ++it1) {
            for (list<Palavra*>::iterator it2 = palavras.begin(); it2 != palavras.end(); ++it2) {
                if (*it1 != *it2) {
                    int dx = (*it1)->x - (*it2)->x;
                    int dy = (*it1)->y - (*it2)->y;
                    int dz = (*it1)->z - (*it2)->z;
                    int distancia = sqrt(dx * dx + dy * dy + dz * dz);

                    if (distancia <= distanciaMaxima) {
                        cout << "Conectando " << (*it1)->nome << " com " << (*it2)->nome
                             << " pela proximidade (" << distancia << ")\n";
                    }
                }
            }
        }
    }

    double calcularSimilaridade(const string &nome1, const string &nome2) {
        Palavra* p1 = NULL;
        Palavra* p2 = NULL;

        for (list<Palavra*>::iterator it = palavras.begin(); it != palavras.end(); ++it) {
            if ((*it)->nome == nome1) p1 = *it;
            if ((*it)->nome == nome2) p2 = *it;
        }

        if (!p1 || !p2) {
            cout << "Uma ou ambas as palavras não foram encontradas.\n";
            return -1.0;
        }

        double dx = p1->x - p2->x;
        double dy = p1->y - p2->y;
        double dz = p1->z - p2->z;

        double distancia = sqrt(dx * dx + dy * dy + dz * dz);
        return 1 / (1 + distancia); // Similaridade inversamente proporcional à distância
    }

    void listarSignificados(const string &nomePalavra) {
        for (list<Palavra*>::iterator it = palavras.begin(); it != palavras.end(); ++it) {
            if ((*it)->nome == nomePalavra) {
                cout << "Significados de " << nomePalavra << ":\n";
                for (list<Significado*>::iterator sigIt = (*it)->significados.begin(); sigIt != (*it)->significados.end(); ++sigIt) {
                    cout << "- " << (*sigIt)->descricao << endl;
                }
                return;
            }
        }

        cout << "Palavra " << nomePalavra << " não encontrada.\n";
    }

    const list<Palavra*>& getPalavras() const {
        return palavras;
    }
};

int main() {
    GrafoDePalavras grafo;
    ArvoreAlfabetica arvoreAlfabetica;
    ArvorePorTamanho arvorePorTamanho;
    int opcao;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Inserir nova palavra\n";
        cout << "2. Conectar palavras por proximidade\n";
        cout << "3. Listar significados de uma palavra\n";
        cout << "4. Exibir palavras em ordem alfabetica\n";
        cout << "5. Exibir palavras ordenadas por tamanho\n";
        cout << "6. Calcular similaridade entre duas palavras\n";
        cout << "7. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
        case 1: {
            string nome;
            int x, y, z;

            cout << "Digite o nome da palavra: ";
            cin >> nome;
            cout << "Digite as coordenadas (x y z) da palavra: ";
            cin >> x >> y >> z;

            grafo.inserirPalavra(nome, x, y, z, arvoreAlfabetica, arvorePorTamanho);
            break;
        }
        case 2: {
            int distanciaMaxima;
            cout << "Digite a distancia máxima: ";
            cin >> distanciaMaxima;
            grafo.conectarPalavrasPorProximidade(distanciaMaxima);
            break;
        }
        case 3: {
            string nomePalavra;
            cout << "Digite o nome da palavra: ";
            cin >> nomePalavra;
            grafo.listarSignificados(nomePalavra);
            break;
        }
        case 4:
            arvoreAlfabetica.exibir();
            break;
        case 5:
            arvorePorTamanho.exibir();
            break;
        case 6: {
            string palavra1, palavra2;
            cout << "Digite o nome da primeira palavra: ";
            cin >> palavra1;
            cout << "Digite o nome da segunda palavra: ";
            cin >> palavra2;

            double similaridade = grafo.calcularSimilaridade(palavra1, palavra2);
            if (similaridade >= 0) {
                cout << "A similaridade entre " << palavra1 << " e " << palavra2 
                     << " é: " << similaridade << endl;
            }
            break;
        }
        case 7:
            cout << "Saindo do programa.\n";
            return 0;
        default:
            cout << "Opção inválida! Tente novamente.\n";
        }
    }
}