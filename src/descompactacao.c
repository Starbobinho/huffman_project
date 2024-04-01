#include <stdio.h>
#include <lista.h>
#include <arvore.h>
#include <descompactacao.h>

// verificação se o inésimo bit é 1 ou 0 e retorna esse valor de 0 ou 1
int is_bit_i_set(unsigned char c, int i){
    unsigned char mask = 1 << i;
    return mask & c;
}
/*
    pega o tamanho do lixo que está nos 3 bits mais significativos 
    do arquivo compactado
*/
int tamanhoLixoArquivoCompactado(FILE *arquivo){

    //lendo primeiro byte do arquivo
    unsigned char byte;
    fread(&byte,sizeof(unsigned char),1,arquivo);

    //empurra 5 bits menos significativos para fora
    short int lixo = byte >> 5;
    fseek(arquivo, 0, SEEK_SET);
    return lixo;
}

/*
    pega o tamanho da árvore que está nos 5 bits menos significativos do primeiro
    byte e nos 8 bits do segundo byte
*/
int tamanhoArvoreArquivoCompactado(FILE *arquivo){
    unsigned char byte;
    fread(&byte,sizeof(unsigned char),1,arquivo);

    //considerando apenas 5 ultimos bits
    byte = byte << 3;
    byte = byte >> 3;
    short int tamanho_arvore = byte;
    tamanho_arvore = tamanho_arvore << 8;
    fread(&byte,sizeof(unsigned char),1,arquivo);
    tamanho_arvore = tamanho_arvore | byte;
    return tamanho_arvore;
}

int tamanhoExtensaoArqCompactado(FILE *arquivo){
    //lendo primeiro byte do arquivo
    unsigned char byte;
    fread(&byte,sizeof(unsigned char),1,arquivo);

    //empurra 5 bits menos significativos para fora
    short int tamanho_extensao = byte >> 5;
    return tamanho_extensao;
}

void recuperarExtensaoArqCompactado(FILE *arquivo, char extensao[], int tamanho_extensao){ 

    for(int i = 0; i < tamanho_extensao; i++){
        fread(extensao+i,sizeof(unsigned char), 1, arquivo);
    }
    extensao[tamanho_extensao] = '\0';
}

/*
    Percorrendo Arvore de Huffman e monta ela sabendo que está em pré ordem
*/
Node *montarArvore(FILE *arquivo,short int *tamanho_arvore){
    unsigned char byte;
    Node *arvore_huffman = NULL;

    //tratando caractere especial (sempre folha)
    if(*tamanho_arvore > 0){
        fread(&byte,sizeof(unsigned char),1,arquivo);
        // caso o byte lido seja uma "\" entramos nesse if por ser um caractere de escape
        if(byte == '\\'){
            // diminuímos o treeSize e lemos o próximo byte
            (*tamanho_arvore)--;
            fread(&byte,sizeof(unsigned char),1,arquivo);
            arvore_huffman = createNode(createVoidPointer(byte),0);
            (*tamanho_arvore)--;
            // Após lermos, diminuímos mais uma vez o treeSize pois já vamos para o próximo byte
            return arvore_huffman;
        }
        arvore_huffman = createNode(createVoidPointer(byte),0);
        (*tamanho_arvore)--;

        //se for um nó intermediario
        if(byte == '*'){
            //busca primeiro na esquerda até achar uma folha
            arvore_huffman->left = montarArvore(arquivo,tamanho_arvore);
            arvore_huffman->right = montarArvore(arquivo,tamanho_arvore);
            // Depois que acharmos cada folha, retornaremos o nó sempre com sua subarvore a esquerda e a direita
            // Assim Construindo e adquirindo nossa árvore de huffman que foi escrita no arquivo
        }
        return arvore_huffman;      
    }
    return arvore_huffman;
}

/*
    Preencher arquivo descompactado
*/
void escreverBytesOriginais(FILE *arquivo_entrada,FILE *arquivo_saida,short int tamanho_lixo,unsigned long long int tamanho_arquivo,Node *arvore_huffman){

    //byte que está sendo manipulado
    unsigned char byte;

    //arvore de huffman
    Node *aux = arvore_huffman;

    //definir até onde ler do byte
    int limite = 0;

    //não considera o header do arquivo
    for(int i = 0;i < tamanho_arquivo;i++){

        //tratamento de lixo no ultimo byte
        if(i == (tamanho_arquivo - 1)) limite = tamanho_lixo;
        
        fread(&byte,sizeof(unsigned char),1,arquivo_entrada);    
        for(int j = 7;j >= limite;j--){

            //se estiver setado vai para direita
            //se não estiver setado vai para esquerda
            if(is_bit_i_set(byte,j)){
                aux = aux->right;             
            }
            else{
                aux = aux->left;
            }

            // quando acha folha imprime no arquivo
            if(isLeaf(aux)){
                unsigned char caractere = getByteFromVoidPointer(aux->byte);
                fwrite(&caractere,sizeof(unsigned char),1,arquivo_saida);
                aux = arvore_huffman;
            }
        }
    }
}
