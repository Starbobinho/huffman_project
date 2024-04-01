#include <stdio.h>
#include <lista.h>
#include <arvore.h>
#include <compactacao.h>

unsigned long long int sizeArchive(FILE *arquivo) {
    // Salva a posição atual do cursor no arquivo
    unsigned long long int posicao_atual = ftell(arquivo);

    // Vá até o final do arquivo
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        perror("Erro ao buscar o final do arquivo");
        return -1; // Retorno de erro
    }
    // Obtenha a posição atual, que é o tamanho do arquivo
    unsigned long long int tamanho = ftell(arquivo);

    if (tamanho == -1) {
        perror("Erro ao obter o tamanho do arquivo");
        return -1; // Retorno de erro
    }

    // Volta para a posição original do cursor no arquivo
    if (fseek(arquivo, posicao_atual, SEEK_SET) != 0) {
        perror("Erro ao retornar para a posição original do arquivo");
        return -1; // Retorno de erro
    }
    return tamanho;
}

void construirTabelaCodigos(Node* no_arvore,BitHuff tabela_codigos[],BitHuff codificacao){         
    if(isLeaf(no_arvore))
    {
        // Montagem do nosso novo mapeamento dos bytes
        tabela_codigos[getByteFromVoidPointer(no_arvore->byte)] = codificacao;
        return;
    }
    else
    {
        //  code.size se trata de quantos bits temos no byte em questão (Útil para escrita posteriormente)
        //  01 -> codificacao.size = 2 || 10 -> code.size = 2 || 11 -> code.size = 2
        codificacao.size++;

        /* 
        Definimos o próximo bit como 0 por garantia primordialmente, pois caso nossa função 'vá' para direita 
        conseguiremos definir o 1 corretamente, caso vá para esquerda o 0 já estará lá para nossa codificação.   
        */ 
        codificacao.bitH <<= 1;
        if(no_arvore->left != NULL)
        construirTabelaCodigos(no_arvore->left, tabela_codigos, codificacao);

        // Seguindo para a direita caso não tenha nenhuma subárvore à esquerda
        codificacao.bitH++;
        if(no_arvore->right != NULL)
        construirTabelaCodigos(no_arvore->right, tabela_codigos, codificacao);
    }        
}

int tamanhoLixo(int frequencia[],BitHuff tabela_codigos[]){
    unsigned long long int total_de_bits = 0;
    for(int i = 0;i < 256;i++){
        if(frequencia[i] == 0) continue;
        total_de_bits += frequencia[i] * tabela_codigos[i].size;
    }
    int lixo = (8 - (total_de_bits % 8));
    return lixo;
}
/*
    Escrevendo o primeiro byte do arquivo
*/
void escreverPrimeiroByte(FILE *arquivo,int tamanho_lixo,int tamanho_arvore){
    unsigned char byte_completo = 0x00;

    // Escrevendo tamanho do lixo nos tres primeiros bits
    byte_completo = byte_completo | (tamanho_lixo << 5);// operação ou bit a bit com o tamanho do lixo

    // Escrevendo os 5 bits mais significativos da árvore
    byte_completo = byte_completo | (tamanho_arvore >> 8);// operação ou bit a bit pra setar os 5 primeiro bits do tamanho da árvore
    
    // Escrevendo o byte completo
    fwrite(&byte_completo,sizeof(unsigned char),1,arquivo);
}
/*
    setando o segundo byte
*/
void escreverSegundoByte(FILE *arquivo,int tamanho_arvore){
    unsigned char byte_completo = 0x00; 

    // setando os 8 bits menos significativos da árvore 
    byte_completo = byte_completo | tamanho_arvore;// operação ou bit a bit com o restante do tamanho da árvore

    fwrite(&byte_completo,sizeof(unsigned char),1,arquivo);
}
/*
    Escrever a árvore no arquivo compactado
*/
void escreverArvore(FILE *arquivo,struct Node *arvore_huffman)
{
    if (arvore_huffman != NULL) {
    unsigned char x = getByteFromVoidPointer(arvore_huffman->byte);

    // Checagem se nossa folha é um caractere especial, se for escreve nosso caractere de escape para depois vir o caractere especial
    if(((x == '*') || (x == '\\')) && isLeaf(arvore_huffman)){
        // y = '\'
        unsigned char y = '\\';
        fwrite(&y,sizeof(unsigned char),1,arquivo);
    }
    // Escreve em pré ordem pois está antes das chamadas recursivas
    fwrite(&x,sizeof(unsigned char),1,arquivo);
    escreverArvore(arquivo,arvore_huffman->left);
    escreverArvore(arquivo,arvore_huffman->right);
    }
}

void escreverExt(FILE *arquivo, char extensao[], int tamanho){
    unsigned char byte = 0x00;
    
    byte = byte | (tamanho << 5);
    fwrite(&byte, sizeof(unsigned char), 1, arquivo);

    for(int i = 1; i <= tamanho; i++){
        byte = 0x00;
        byte = byte | extensao[i];
        fwrite(&byte, sizeof(unsigned char), 1, arquivo);
    }
}

void escreverBytesCodificados(FILE *arquivo_entrada,FILE *arquivo_saida,BitHuff tabela_codigos[],int tamanho_lixo){
    // buffer size é o tamanho do que já foi setado
    // caractere é o que vai receber os bytes do arquivo
    short tamanho_buffer = 0, caractere;
    // buffer é o que vai ser setado no arquivo
    unsigned char buffer = 0; 
    // aux é o tamanho do que a gente não conseguiu setar e o code_temp vai guardar primeiramente 
    // o que a gente consegue setar depois o que não conseguimos setar como short int(2 bytes)
    unsigned short int aux, code_tmp = 0;
    while((caractere = getc(arquivo_entrada)) != EOF)
    {        
        // checa se conseguimos setar todos os bits no buffer
        if((tabela_codigos[caractere].size + tamanho_buffer) <= 8)
        {
            buffer <<= tabela_codigos[caractere].size; // abre espaço para setar a nova codificação
            buffer = buffer | tabela_codigos[caractere].bitH;// seta a nova codificação no buffer
            tamanho_buffer += tabela_codigos[caractere].size;// aumenta o tamanho pra dizer quanto já foi setado
        }    
        // caso não consiga setar todos    
        else 
        {   
            aux = tabela_codigos[caractere].size;                         
            buffer <<= (8 - tamanho_buffer);// abre espaço para o quanto a gente CONSEGUE setar
            code_tmp = tabela_codigos[caractere].bitH >> (tabela_codigos[caractere].size - (8 - tamanho_buffer));// code_tmp o quanto a gente consegue setar
            aux -= (8 - tamanho_buffer);// tamanho do que a gente não conseguiu setar(o resto)            
            buffer = buffer | code_tmp;
            fprintf(arquivo_saida, "%c", buffer);
            buffer = 0;
            tamanho_buffer = aux;// buffer size vai receber o tamanho do resto                
            code_tmp = tabela_codigos[caractere].bitH << (16 - tamanho_buffer); // Deslocamento para esquerda para preparar para colocar no buffer
            code_tmp >>= (16 - tamanho_buffer); // Voltando os bits para se adequar ao tamanho do buffer
            buffer = code_tmp; // Armazena para próxima interação do loop                   
        }
        // se o buffer está cheio
        if(tamanho_buffer == 8)
        {           
            fprintf(arquivo_saida, "%c", buffer);
            buffer = 0;
            tamanho_buffer = 0;
        }
    }    
    // para caso tenha lixo 
    if(tamanho_buffer > 0 && tamanho_buffer < 8)
    {         
        buffer <<= tamanho_lixo;//joga os bits setados pro começo do byte     
        fprintf(arquivo_saida, "%c", buffer);
        buffer = 0;
        tamanho_buffer = 0; 
    }
}