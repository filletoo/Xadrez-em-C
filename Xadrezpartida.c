#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <Windows.h>
#define BRANCO 0
#define PRETO 1
#define CAMINHO_MAPA "tabuleiro.txt"
#define SKINS "ptkndrPTKNDR "

typedef struct{
    char tipo;
    int valor;
    int cor;
    int pos[2];
    int H[2]; int V[2];
    int D[4];
    int E[65][2];
    int MoveVal[65][2];
} Peca;

Peca Pvazio = {' ', 0, -1};

int stdtoi(char c){
    if (c >= 'a' && c <= 'h'){
        return c - 'a';
    }

    return 8 - (c - '0');
}

char itostd(int i, char c){
    if (c == 'l'){
        return (char)(8 - i + '0');
    }
    return (char)(i + 'a');
}

int valinp(char inp[3]){
    if (!(inp[0] >= 'a' && inp[0] <= 'h' && inp[1] >= '1' && inp[1] <= '8')) return 0;
    return 1;
}

int existemoveval(Peca *p, int linha, int coluna){
    for (int i = 1; i <= p->MoveVal[0][0]; i++){
        if (p->MoveVal[i][0] == linha && p->MoveVal[i][1] == coluna) return 1;
    }

    return 0;
}

void movepeao(Peca *p, Peca tabuP[8][8], char inp_org_ant[3]){
    int enpassant[4] = {0,0,0,0};
    if (p->pos[0] != 6 && p->cor == BRANCO){
        if (tabuP[p->pos[0] - 1][p->pos[1]].cor == PRETO) p->V[1] = 0;
        else p->V[1] = 1;
    }else if (p->cor == BRANCO){
        p->V[1] = 2;
        if (tabuP[p->pos[0] - 2][p->pos[1]].cor == PRETO) p->V[1] = 1;
    }
    if (p->pos[0] != 1 && p->cor == PRETO){
        if (tabuP[p->pos[0] + 1][p->pos[1]].cor == BRANCO) p->V[0] = 0;
        else p->V[0] = 1;
    }else if (p->cor == PRETO){
        p->V[0] = 2;
        if (tabuP[p->pos[0] + 2][p->pos[1]].cor == BRANCO) p->V[0] = 1;
    }

    //en passant
    if (p->cor == BRANCO && p->pos[0] == 3){
        if (inp_org_ant[1] == '7'){
            if (p->pos[1] + 1 < 8 && tabuP[p->pos[0]][p->pos[1] + 1].tipo == 'p' && stdtoi(inp_org_ant[0]) == p->pos[1] + 1)
                p->D[0] = enpassant[0] = 1; 
            if (p->pos[1] - 1 >= 0 && tabuP[p->pos[0]][p->pos[1] - 1].tipo == 'p' && stdtoi(inp_org_ant[0]) == p->pos[1] - 1)
                p->D[1] = enpassant[1] = 1; 
        }
    }else if (p->cor == PRETO && p->pos[0] == 4){
        if (inp_org_ant[1] == '2'){
            if (p->pos[1] + 1 < 8 && tabuP[p->pos[0]][p->pos[1] + 1].tipo == 'P' && stdtoi(inp_org_ant[0]) == p->pos[1] + 1)
                p->D[3] = enpassant[3] = 1; 
            if (p->pos[1] - 1 >= 0 && tabuP[p->pos[0]][p->pos[1] - 1].tipo == 'P' && stdtoi(inp_org_ant[0]) == p->pos[1] - 1)
                p->D[2] = enpassant[2] = 1; 
        }
    }

    if (p->pos[0] == 0 || p->pos[0] == 7) return;

    if (p->pos[1] != 7){
        //diagonal 1
        if (p->cor == BRANCO && tabuP[p->pos[0] - 1][p->pos[1] + 1].cor == PRETO ){
            p->D[0] = 1;
        }else if (!enpassant[0]) p->D[0] = 0;
        //diagonal 4
        if (p->cor == PRETO && tabuP[p->pos[0] + 1][p->pos[1] + 1].cor == BRANCO ){
            p->D[3] = 1;
        }else if (!enpassant[3]) p->D[3] = 0;
    }
    if (p->pos[1] != 0){
        //diagonal 2
        if (p->cor == BRANCO && tabuP[p->pos[0] - 1][p->pos[1] - 1].cor == PRETO ){
            p->D[1] = 1;
        }else if (!enpassant[1]) p->D[1] = 0;
        //diagonal 3
        if (p->cor == PRETO && tabuP[p->pos[0] + 1][p->pos[1] - 1].cor == BRANCO ){
            p->D[2] = 1;
        }else if (!enpassant[2]) p->D[2] = 0;
    }
}

void listarMove(Peca *p, Peca tabuP[8][8]){
    int n = 1;
    int lim, movido;
    if (tolower(p->tipo) == 'p') movepeao(p, tabuP, "a5");
    //horizontal direita
    lim = p->H[1]; movido = 0;
    for (int i = p->pos[1] + 1; i < 8 && movido < lim; i++){
        if (tabuP[p->pos[0]][i].cor != p->cor){
            p->MoveVal[n][1] = i;
            p->MoveVal[n][0] = p->pos[0];
            movido++; n++;
            if (tabuP[p->pos[0]][i].cor != -1) break;
        }else break;
    }
    //horizontal esquerda
    lim = p->H[0]; movido = 0;
    for (int i = p->pos[1] - 1; i >= 0 && movido < lim; i--){
        if (tabuP[p->pos[0]][i].cor != p->cor){
            p->MoveVal[n][1] = i;
            p->MoveVal[n][0] = p->pos[0];
            movido++; n++;
            if (tabuP[p->pos[0]][i].cor != -1) break;
        }else break;
    }
    //vertical cima
    lim = p->V[1]; movido = 0;
    for (int i = p->pos[0] - 1; i >= 0 && movido < lim; i--){
        if (tabuP[i][p->pos[1]].cor != p->cor){
            p->MoveVal[n][1] = p->pos[1];
            p->MoveVal[n][0] = i;
            movido++; n++;
            if (tabuP[i][p->pos[1]].cor != -1) break;
        }
        else break;
    }
    //vertical baixo
    lim = p->V[0]; movido = 0;
    for (int i = p->pos[0] + 1; i < 8 && movido < lim; i++){
        if (tabuP[i][p->pos[1]].cor != p->cor){
            p->MoveVal[n][1] = p->pos[1];
            p->MoveVal[n][0] = i;
            movido++; n++;
            if (tabuP[i][p->pos[1]].cor != -1) break;
        }
        else break;
    }
    //diagonal 1
    lim = p->D[0]; movido = 0;
    for (int i = p->pos[0] - 1, j = p->pos[1] + 1; i >= 0 && j < 8 && movido < lim; i--, j++){
        if (tabuP[i][j].cor != p->cor){
            p->MoveVal[n][0] = i;
            p->MoveVal[n][1] = j;
            movido++; n++;
            if (tabuP[i][j].cor != -1) break;
        }else break;
    }
    //diagonal 2
    lim = p->D[1]; movido = 0;
    for (int i = p->pos[0] - 1, j = p->pos[1] - 1; i >= 0 && j >= 0 && movido < lim; i--, j--){
        if (tabuP[i][j].cor != p->cor){
            p->MoveVal[n][0] = i;
            p->MoveVal[n][1] = j;
            movido++; n++;
            if (tabuP[i][j].cor != -1) break;
        }else break;
    }
    //diagonal 3
    lim = p->D[2]; movido = 0;
    for (int i = p->pos[0] + 1, j = p->pos[1] - 1; i <8  && j >= 0 && movido < lim; i++, j--){
        if (tabuP[i][j].cor != p->cor){
            p->MoveVal[n][0] = i;
            p->MoveVal[n][1] = j;
            movido++; n++;
            if (tabuP[i][j].cor != -1) break;
        }else break;
    }
    //diagonal 4
    lim = p->D[3]; movido = 0;
    for (int i = p->pos[0] + 1, j = p->pos[1] + 1; i < 8 && j < 8 && movido < lim; i++, j++){
        if (tabuP[i][j].cor != p->cor){
            p->MoveVal[n][0] = i;
            p->MoveVal[n][1] = j;
            movido++; n++;
            if (tabuP[i][j].cor != -1) break;
        }else break;
    }

    //especiais
    movido = p->E[0][0];
    int pos_lin, pos_col;
    for (int i = 1; i <= movido; i++){
        pos_lin =  p->E[i][0];
        pos_col = p->E[i][1];
        if (p->cor == PRETO){
            pos_lin *= -1;
            pos_col *= -1;
        }
        pos_lin += p->pos[0];
        pos_col += p->pos[1];
        if (pos_lin >= 0 && pos_lin < 8 && pos_col >= 0 && pos_col < 8 && tabuP[pos_lin][pos_col].cor != p->cor){
            p->MoveVal[n][0] = pos_lin;
            p->MoveVal[n][1] = pos_col;
            n++;
        }
    }
    p->MoveVal[0][0] = n - 1;
}

void printtabu(char tabuC[9][19]){
    for (int i = 0; i < 9; i++){
        printf("%s\n", tabuC[i]);
    }
}

void atlztabuC(char tabuC[9][19], Peca tabuP[8][8]){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 18; j++){
            if (strrchr(SKINS, tabuC[i][j]) != NULL || tabuC[i][j] == 'x'){
                tabuC[i][j] = tabuP[i][j/2 - 1].tipo;
            }
        }
    }
}

void atlzcomx(char org[3], Peca tabuP[8][8], char tabuC[9][19]){
    Peca obj;
    obj = tabuP[stdtoi(org[1])][stdtoi(org[0])];

    for (int i = 1; i <= obj.MoveVal[0][0]; i++){
        tabuC[obj.MoveVal[i][0]][2*(obj.MoveVal[i][1] + 1)] = 'x';
    }
    /*for (int i = 0; i < 8; i++){
        for (int j = 0; j < 18; j++){
            if (existemoveval(&obj, i, j/2 - 1)) printf("x");
            else printf("%c", tabuC[i][j]);
        }
        printf("\n");
    }
    printf("%s\n", tabuC[8]);*/
}

int moverstd(char org[3], char dest[3], Peca tabuP[8][8]){
    int lin_org, col_org, lin_dest, col_dest;

    lin_org = stdtoi(org[1]); col_org = stdtoi(org[0]);
    lin_dest = stdtoi(dest[1]); col_dest = stdtoi(dest[0]);

    //if (!existemoveval(&tabuP[lin_org][col_org], lin_dest, col_dest)) return 0;

    tabuP[lin_dest][col_dest] = tabuP[lin_org][col_org];
    tabuP[lin_dest][col_dest].pos[0] = lin_dest;
    tabuP[lin_dest][col_dest].pos[1] = col_dest;

    //enpassant
    Peca obj = tabuP[lin_dest][col_dest];
    if (obj.tipo == 'p' && tabuP[lin_dest - 1][col_dest].tipo == 'P'){
        tabuP[lin_dest - 1][col_dest] = Pvazio;
    }else if (obj.tipo == 'P' && tabuP[lin_dest + 1][col_dest].tipo == 'p'){
        tabuP[lin_dest + 1][col_dest] = Pvazio;
    }
    tabuP[lin_org][col_org] = Pvazio;
    return 1;
}

int moverpos(int org[2], int dest[2], Peca tabuP[8][8]){
    int lin_org, col_org, lin_dest, col_dest;

    lin_org = org[0]; col_org = org[1];
    lin_dest = dest[0]; col_dest = dest[1];

    //if (!existemoveval(&tabuP[lin_org][col_org], lin_dest, col_dest)) return 0;

    tabuP[lin_dest][col_dest] = tabuP[lin_org][col_org];
    tabuP[lin_dest][col_dest].pos[0] = lin_dest;
    tabuP[lin_dest][col_dest].pos[1] = col_dest;

    //enpassant
    Peca obj = tabuP[lin_dest][col_dest];
    if (obj.tipo == 'p' && tabuP[lin_dest - 1][col_dest].tipo == 'P'){
        tabuP[lin_dest - 1][col_dest] = Pvazio;
    }else if (obj.tipo == 'P' && tabuP[lin_dest + 1][col_dest].tipo == 'p'){
        tabuP[lin_dest + 1][col_dest] = Pvazio;
    }
    tabuP[lin_org][col_org] = Pvazio;
    return 1;
}

void configpeca(Peca *tempP, char temp){
    tempP->tipo = temp;
    if (temp >= 'a' && temp <= 'z') tempP->cor = PRETO;
    else if (temp != ' ') tempP->cor = BRANCO;

    if (tolower(temp) == 't') {
        tempP->valor = 5;
        tempP->V[0] = 9; tempP->V[1] = 9;
        tempP->H[0] = 9; tempP->H[1] = 9;
    }
    else if (tolower(temp) == 'k') {
        int n = 1;
        tempP->valor = 3;
        tempP->E[1][0] = -2; tempP->E[1][1] = -1; 
        tempP->E[2][0] = -2; tempP->E[2][1] = 1; 
        tempP->E[3][0] = 2; tempP->E[3][1] = -1; 
        tempP->E[4][0] = 2; tempP->E[4][1] = 1;
        tempP->E[5][0] = -1; tempP->E[5][1] = -2; 
        tempP->E[6][0] = -1; tempP->E[6][1] = 2; 
        tempP->E[7][0] = 1; tempP->E[7][1] = -2; 
        tempP->E[8][0] = 1; tempP->E[8][1] = 2; 
        /*for (int k = 0; k < 2; k++){
            for (int i = -2; i <= 2; i += 4){
                for (int j = 0; j < 2; j++){
                    tempP->E[n][k] = i;
                    n++;
                }
            }
        }
        n = 1;
        for (int k = 1; k >= 0; k--){
            for (int i = -1; i <= 1; i += 2){
                for (int j = 0; j < 2; j++){
                    tempP->E[n][k] = i;
                    n++;
                }
            }
        }
        tempP->E[0][0] = n - 1;*/
        tempP->E[0][0] = 8;
    }
    else if (temp == 'p') {
        tempP->valor = 1;
        tempP->V[0] = 2;
    }
    else if (temp == 'P') {
        tempP->valor = 1;
        tempP->V[1] = 2;
    }
    else if (tolower(temp) == 'n') {
        tempP->valor = 3;
        tempP->D[0] = 9; tempP->D[1] = 9;
        tempP->D[2] = 9; tempP->D[3] = 9;
    }
    else if (tolower(temp) == 'd') {
        tempP->valor = 9;
        tempP->D[0] = 9; tempP->D[1] = 9;
        tempP->D[2] = 9; tempP->D[3] = 9;
        tempP->V[0] = 9; tempP->V[1] = 9;
        tempP->H[0] = 9; tempP->H[1] = 9;
    }
    else if (tolower(temp) == 'r') {
        tempP->D[0] = 1; tempP->D[1] = 1;
        tempP->D[2] = 1; tempP->D[3] = 1;
        tempP->V[0] = 1; tempP->V[1] = 1;
        tempP->H[0] = 1; tempP->H[1] = 1;
    }
}

void promocao(Peca *p){
    char apromo;
    if (p->pos[0] == 0 && p->tipo == 'P' || p->pos[0] == 7 && p->tipo == 'p'){
        printf("Promocao: ");
        scanf(" %c", &apromo);
        if (p->cor == BRANCO && apromo >= 'a' && apromo <= 'z') apromo -= 'a' - 'A';
        else if (p->cor == PRETO && apromo >= 'A' && apromo <= 'Z') apromo += 'a' - 'A';
        configpeca(p, apromo);
    }
}

int emcheck(Peca rei, Peca tabuP[8][8]){
    int lin = rei.pos[0], col = rei.pos[1];
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (tabuP[i][j].tipo != ' '){
                if (rei.cor != tabuP[i][j].cor && existemoveval(&tabuP[i][j], lin, col)) return 1;
            }
        }
    }
    return 0;
}

void simullistarMove(Peca *p, Peca tabuP[8][8]){
    Peca c_tabuP[8][8];
    Peca rei;
    int *n = &p->MoveVal[0][0];
    //memcpy(c_tabuP, tabuP, sizeof(Peca)*64);
    
    for (int i = 1; i <= p->MoveVal[0][0]; i++){
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++) c_tabuP[i][j] = tabuP[i][j];
        }
        moverpos(p->pos, p->MoveVal[i], c_tabuP);
        for (int j = 0; j < 8; j++){
            for (int k = 0; k < 8; k++) {
                if (c_tabuP[j][k].tipo != ' '){
                    if (c_tabuP[j][k].cor != p->cor)
                        listarMove(&c_tabuP[j][k], c_tabuP);
                    else if (tolower(c_tabuP[j][k].tipo) == 'r') rei = c_tabuP[j][k];
                }
            }
        }
        if (emcheck(rei, c_tabuP)){
            p->MoveVal[i][0] = p->MoveVal[*n][0];
            p->MoveVal[i][1] = p->MoveVal[*n][1];
            (*n)--; i--;
        }
    }
}

int main (){
    FILE *mapa_t = fopen(CAMINHO_MAPA, "r");
    FILE *partida = fopen("partida2.txt", "r");
    char tabuC[9][19];
    Peca tabuP[8][8];
    Peca tempP;
    int pontos_branco, pontos_preto;

    //pegar o mapa em strings
    for (int i = 0; i < 9; i++){
        fgets(tabuC[i], 19, mapa_t);
        fgetc(mapa_t);
    }

    //pegar o mapa em tipo Pecas
    char temp;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 18; j++){
            temp = tabuC[i][j];
            if (strrchr(SKINS, temp) != NULL){
                tempP = Pvazio;
                //tipo
                tempP.tipo = temp;
                //cor
                //posicao
                tempP.pos[0] = i;
                tempP.pos[1] = j/2 - 1;
                configpeca(&tempP, temp);
                //fim
                tabuP[i][j/2 - 1] = tempP;
            }
        }
    }

    Peca *porg, *pdest;
    Peca rei;
    int vez = BRANCO;
    char inp_org[3], inp_dest[3];
    char inp_org_ant[3], inp_dest_ant[3];
    int qtdmove, check;
    DWORD delay = 1000;

    while (1){
        printtabu(tabuC);
        //pontuação
        pontos_branco = 0; pontos_preto = 0;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (tabuP[i][j].cor == BRANCO) pontos_branco += tabuP[i][j].valor;
                else if (tabuP[i][j].cor == PRETO) pontos_preto += tabuP[i][j].valor;      
            }
        }
        if (pontos_branco >= pontos_preto) printf("Branco: +%d\n", pontos_branco - pontos_preto);
        else printf("Preto: +%d\n", pontos_preto - pontos_branco);

        //em check
        qtdmove = 0;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (tabuP[i][j].cor + vez == 1) listarMove(&tabuP[i][j], tabuP);
                if (tolower(tabuP[i][j].tipo) == 'r' && tabuP[i][j].cor != vez) 
                    rei = tabuP[i][j];
                if (tabuP[i][j].cor == vez){
                    listarMove(&tabuP[i][j], tabuP);
                    simullistarMove(&tabuP[i][j], tabuP);
                    qtdmove += tabuP[i][j].MoveVal[0][0];
                }
            }
        }
        check = emcheck(rei, tabuP);
        if (qtdmove == 0){
            if (check){
                printf("Checkmate!\n");
                break;
            }else {
                printf("Afogamento!\n");
                break;
            }
        }else if (check) printf("Check!\n");

        Sleep(delay);
        //ler input
        do{
            fscanf(partida, "%2s", inp_org);
            if (strcmp(inp_org, "x") == 0) break;
            if (!valinp(inp_org)) printf("Input invalido\n");
        }while (!valinp(inp_org));
        if (strcmp(inp_org, "x") == 0) break;
        porg = &tabuP[stdtoi(inp_org[1])][stdtoi(inp_org[0])];
        //printf("%s\n", inp_org);
        //system("pause");
        //verificar vez
        /*if (porg->cor != vez) {
            printf("Nao e sua vez\n");
            continue;
        }*/

        //movimento do peao
        if (tolower(porg->tipo) == 'p'){
            movepeao(porg, tabuP, inp_org_ant);
        }
        
        listarMove(porg, tabuP);
        simullistarMove(porg, tabuP);

        atlzcomx(inp_org, tabuP, tabuC);
        printtabu(tabuC);

        Sleep(delay);
        do{
            fscanf(partida, "%2s", inp_dest);
            if (!valinp(inp_dest)) printf("Input invalido\n");
        }while (!valinp(inp_dest));
        pdest = &tabuP[stdtoi(inp_dest[1])][stdtoi(inp_dest[0])];
        //printf("%s\n", inp_dest);
        //system("pause");
        //movimento
        if(!moverstd(inp_org, inp_dest, tabuP)) printf("Movimento Invalido\n");
        else{
            promocao(pdest);
            if (vez == BRANCO) vez = PRETO;
            else vez = BRANCO;
        }

        strcpy(inp_org_ant, inp_org); strcpy(inp_dest_ant, inp_dest);
        atlztabuC(tabuC, tabuP);
    }

    system("pause");
    fclose(mapa_t);
    fclose(partida);
    return 0;
}