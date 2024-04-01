#include <stdio.h>
#include <lista.h>
#include <arvore.h>
#include <compactacao.h>
#include <descompactacao.h>

#define MAX 100

int main() {
    int i;
    printf("SEJA BEM VINDO A CODIFICAÇÃO DE HUFFMAN\n");
    printf("SELECIONE UMA DAS OPÇÕES A SEGUIR:\n");
    printf("1 - COMPACTAR ARQUIVO\n");
    printf("2 - DESCOMPACTAR ARQUIVO\n");
    printf("OPÇÃO: ");
    scanf("%d",&i);
    if(i == 1){
        FILE *arquivo;
        char caminho[MAX]; // Array que armazena o nome do arquivo
        int tamanho_atual = 0;

        printf("Digite o caminho da pasta do arquivo: ");
        scanf("%s", caminho);
        // Inicialização da lista encadeada para armazenar as frequencias em ordem crescente
        struct Node* lista_frequencia = createList();

        // Abre para leitura dos bytes do arquivo
        arquivo = fopen(caminho, "rb");
        unsigned long long int tamanho_1 = sizeArchive(arquivo);
        
        // Bloco de código para leitura e armazenamento da extensao do nosso arquivo +condição de max de 6 linhas:32-45
        char extensao[6];
        int tam_ext = -1;
        char *cursor = strrchr(caminho, '.');
        while(*cursor != '\0'){
            extensao[tam_ext + 1] = *cursor;
            cursor++;
            tam_ext++;
        }
        if(tam_ext > 6){
            printf("Tamanho de extensão de arquivo inválida\nTamanho máximo = 6, Tamanho fornecido = %d\n", tam_ext);
            printf("\n Encerrando programa. . . \n");
            return 1;
        }

        // Remove a extensão do arquivo e adiciona .huff
        char *remove = strrchr(caminho,'.');
        if(remove != NULL) *(remove) = '\0';
        strcat(caminho,".huff");

        // Abre o arquivo para escrita
        FILE *arquivo_saida = fopen(caminho,"wb");
        if (arquivo_saida == NULL)
        {
            perror("Erro ao abrir o arquivo");
            return 1;
        }
        // Caso de erro na leitura do arquivo
        if (arquivo == NULL)
        {
            perror("Erro ao abrir o arquivo");
            return 1;
        }

        // Array armazenando a frequencia dos bytes
        int frequencia_bytes[256] = {0};

        // Função que conta essa frequencia de bytes e armazena no nosso array
        adquirirFrequencia(arquivo,frequencia_bytes);

        // Função para inserir a frequencia em uma lista encadeada, a partir do nosso array frequencia_bytes
        inserirFrequencia(&lista_frequencia, frequencia_bytes,&tamanho_atual);

        // Função para montar nossa árvore partindo da nossa lista encadeada que foi ordenada do menor para o maior
        arvoreDeHuffman(&lista_frequencia,&tamanho_atual);

        // Linha 78-82, são inicializações das nossas variáveis e da nossa tabela de códigos tudo em 0 essencialmente para trabalharmos em cima disso
        BitHuff byte_de_huffman,tabela_codigos[256];
        byte_de_huffman.bitH = 0;
        byte_de_huffman.size = 0;
        memset(tabela_codigos,0,sizeof(BitHuff) * 256);
        construirTabelaCodigos(lista_frequencia,tabela_codigos,byte_de_huffman);

        // Linhas 85-88 escrevemos o cabeçalho do nosso arquivo compactado
        escreverPrimeiroByte(arquivo_saida,tamanhoLixo(frequencia_bytes,tabela_codigos),tamanhoArvore(lista_frequencia));
        escreverSegundoByte(arquivo_saida,tamanhoArvore(lista_frequencia));
        escreverArvore(arquivo_saida,lista_frequencia);
        escreverExt(arquivo_saida,extensao, tam_ext);

        // Voltando o cursor para o começo do arquivo principal e assim começando a codificar os bytes desse arquivo
        fseek(arquivo, 0, SEEK_SET);
        escreverBytesCodificados(arquivo,arquivo_saida,tabela_codigos,tamanhoLixo(frequencia_bytes,tabela_codigos));

        unsigned long long int tamanho2 = sizeArchive(arquivo_saida);
        fclose(arquivo);
        fclose(arquivo_saida);

        printf("TAMANHO DO ARQUIVO ORIGINAL EM BYTES: %llu\n",tamanho_1);
        printf("TAMANHO DO ARQUIVO COMPACTADO EM BYTES: %llu\n",tamanho2);
        if(tamanho_1 > tamanho2){
            printf("A COMPRESSÃO SE DEMONSTROU EFICIENTE\n");
        }
        else{
            printf("A COMPRESSÃO SE DEMONSTROU INEFICIENTE\n");
        }
    }
    else if(i == 2){
        // Iniciando nossos arquivos que serão trabalhados e armazenando caminho do compactado
        FILE *arquivo_compactado;
        FILE *arquivo_saida;
        char caminho[MAX];
        printf("Digite o caminho da pasta do arquivo: ");
        scanf("%s", caminho);

        // Finalmente abrindo nosso arquivo compactado em modo de leitura de bits
        arquivo_compactado = fopen(caminho, "rb");

        // Removendo o ".huff" para posteriormente adicionarmos a extensão original do arquivo que está no cabeçalho
        char *remove = strrchr(caminho,'.');
        if(remove != NULL) *(remove + 1) = '\0';        

        // Leitura do nosso cabeçalho (2 primeiros bytes)
        short int tamanho_lixo = tamanhoLixoArquivoCompactado(arquivo_compactado);
        short int tamanho_arvore = tamanhoArvoreArquivoCompactado(arquivo_compactado);

        // Calculando tamanho do arquivo e desconsiderando o cabeçalho
        unsigned long long int tamanho_arquivo_compactado = sizeArchive(arquivo_compactado);
        tamanho_arquivo_compactado -= 2;
        tamanho_arquivo_compactado -= tamanho_arvore;

        // Monta nossa árvore de huffman que foi escrita no cabeçalho
        Node *arvore_de_huffman = montarArvore(arquivo_compactado,&tamanho_arvore);

        /* 
            Cálculo do tamanho da extensão do arquivo, armazenamento dessa extensão a partir do arquivo compactado em um array
            após isso concatenamos na nossa string do caminho do arquivo a extensão original do arquivo
            Linhas 139-150
        */
        int tamanho_extensao = tamanhoExtensaoArqCompactado(arquivo_compactado);
        tamanho_arquivo_compactado -= (tamanho_extensao + 1);
        if(tamanho_extensao > 6){
            printf("Tamanho de extensão de arquivo inválida\nTamanho máximo = 6, Tamanho fornecido = %d\n", tamanho_extensao);
            printf("\n Encerrando programa. . . \n");
            return 1;
        }
        char extensao[tamanho_extensao+1];
        recuperarExtensaoArqCompactado(arquivo_compactado, extensao, tamanho_extensao);
        strcat(caminho, extensao);

        arquivo_saida = fopen(caminho, "wb");

        // Função autoexplicativa, Escreve os Bytes do arquivo original a partir do arquivo compactado
        escreverBytesOriginais(arquivo_compactado,arquivo_saida,tamanho_lixo,tamanho_arquivo_compactado,arvore_de_huffman);
        
        fclose(arquivo_compactado);
        fclose(arquivo_saida);
    }
    
    return 0;
}
