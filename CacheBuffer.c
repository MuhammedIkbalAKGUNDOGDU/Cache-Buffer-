#include <stdio.h>
#include <stdlib.h>
#define T 10  // T degiskeni basa atama icin sayi
#define L 5   // L degiskeni max dugum sayisi

typedef struct node{
    char *variable;     //dosyadan alinan satirlar ya da girilen data
    int counter;        //kac adet ayni veriden var saymak icin counter
    struct node *next;  //sonraki dugum dugum
    struct node *prev;  //onceki dugum
} node;

node *head = NULL; // head degiskeni her yerde kullanacagim icin global tanimladim

int main() {
    int choice,choice2; //ane menu icin gerekli degiskenler
    char input[50];
    printf("Welcome To Cache Buffer Simulator\n"); // giris menusu
    printf("Please Choose Your Operation :\n");
    printf("1-Inputs From File\n");
    printf("2-Inputs With Hand\n");
    scanf("%d",&choice);
    while(choice < 1 || choice > 2){                   //dogru giris yapildi mi kontrol
        printf("Invalid Operation !! Please Choose Valid Operation:\n");
        printf("1-Inputs From File\n");
        printf("2-Inputs With Hand\n");
        scanf("%d",&choice);
    }
    if(choice ==1)                                                   //metinden tek seferde input alma
    {
        printf("Please Enter Your File name like input.txt :");
        scanf("%s",input);
        inputFromFile(input);                                          //dosya ile alma fonksiyonu
    }else{                                                              //elle giris
            char *ptr;
            ptr = (char *) calloc(100, sizeof(char));
        do{
            printf("If You Want To Exit Enter 0\n");                    // metin giris 0 girilirse cikis yapilir
            printf("Enter Your input : \n");
            scanf("%s",ptr);
        if(ptr !=0){
             inputWithHand(ptr);                //elle giris fonksiyonu
        }
        }while(strcmp("0",ptr)!=0);     //0 girildi mi kontrolu
    }
    printf("1- Yes 2-No\n");
    printf("Do You Want To Clear Cache Buffer LinkedList : ");          //cache bufferi temizlemek istiyor musun sorusu
    scanf("%d",&choice2);
    if (choice2==1){
        clearList();                                                    //temizle
        printf("Cache Cleared\n");
        print();//temizlendigini gostermek icin tekrardan cache bufferi yazdiriyorum ve bir sey cikmiyor temizlendigine kanit niteliginde
    }else{
        printf("Have a Nice Day");           // normal cikis
    }

    return 0;
}

void inputFromFile(char *input) {
    FILE *file = fopen(input, "r"); // dosya aciyorum okuma modunda

    if (file == NULL) {
        printf("Please Check Your File Name !!!\n");
        return;
    }

    char line[100]; // dosyadan okunacak satirlari bu dizide tutuyorum

    while (fgets(line, sizeof(line), file) != NULL) {// satirlar okunuyorsa islemlere gec
        char *ptr = strtok(line, " \n"); // satir sonuna gelince assagi satira gecme komutu ayari

        while (ptr != NULL) {
            // her satir tek tek burada ekleniyor
            node *newNode = (node *)malloc(sizeof(node)); // yer ayirma islemi
            newNode->variable = strdup(ptr); // satiri yeni dugumun variablesine yani data kismine kopyalama
            newNode->counter = 1;   //counter ayar
            add(newNode);           // add fonksiyonu ana dugum ekleme fonksiyonu

            print(); // her yeni dugum eklemesin  de print islemi gerceklesiyor

            ptr = strtok(NULL, " \n"); // sonraki satira atla
        }
    }

    fclose(file); // dosya kapat
    printf("File Closed\n"); // dosya kapandi mesaji
}

void inputWithHand(char *ptr){             // elle giris
    node *try1 = (node *) calloc(1, sizeof(node));      // memory allocation
    try1->variable = strdup(ptr);            // giris verisini kopyaladim variable kismina
    try1->counter = 1;            // counter ayar
    add(try1);                  // ana dugum ekleme islemi
    print();            // yazdirma
}

void add(node *newNode) {           //ana ekleme fonksiyonu
    if (head == NULL) {
        insertAtFront(newNode);                 // head bossa direkt en basa ekle dugumu
    } else {
        node *tmp = head;                           //degilse headi tmpye ata
        int flag = 1;
        while (tmp != NULL && flag == 1) {   //ayni degiskenden var mi ona bakiyoruz varsa flag ile cikis yoksa cikip yeni dugum atamasi olur

            if (strcmp(tmp->variable, newNode->variable) == 0) { // ayni veri var mi bakiyoruz
                tmp->counter++;
                if(tmp->counter >= T)                               //varsa esik degerini gecti mi kontrol ediyoruz
                {
                    thresholdAtFront(tmp);                          //gecerse en basa atama fonksiyonu
                }

                flag = 0;                          //cikis icin flag degistirme
            }
            tmp = tmp->next;            //tüm dugumleri dolasmak icin mekanizma
        }
        if (flag == 1) {                //ciktiysak ve flag 1 ise demek ki yeni dugum
            newNode->counter = 1;             //counter ayari
            insertAtFront(newNode);         // en basa atama fonksiyonu
        }
    }
}

void print() {              //tek tek her dugumu verisi ve counteri ile birlikte alt alta yazdiriyoruz
    node *tmp = head;
    while (tmp != NULL) {
        printf("Value : %s Counter: %d \n", tmp->variable, tmp->counter);
        tmp = tmp->next;
    }
    printf("*****\n");      // sona geldik demek
}

void insertAtFront(node *newNode) {     // basa ekleme fonksiyonu
    newNode->next = head;               //yeni dugumun nexti head oluyor
    newNode->prev = NULL;                   // yeni dugumun oncesi null oluyor
    if (head != NULL) {         //head bos degilse  headin previ new node oluyor
        (head)->prev = newNode;
    }
    head = newNode;         //head artık new node gerekli baglamalar da yapildi
    if(sizeOfList()>L){         //size of list komutu kac dugum var ona bakiyor L yi gecerse silme oluyor
        delete();           //silme komutu
    }
}

void thresholdAtFront(node *newNode){  // esik degeri gecildi en basa atama

    int flag =1;        //kontrol degiskeni
    node *tmp;
    tmp = head;     //tmp ye head atandi

    while(tmp != NULL && flag == 1){    // aranana degisken bulana kadar gez

        if(strcmp(tmp->variable, newNode->variable) == 0){      // bulundu
            flag = 0;
            tmp->prev->next =tmp->next;//aradan cikarildi ve gerekli baglamalar yapildi
            insertAtFront(newNode);     //en basa atandi
        }
        tmp = tmp->next;    //tum dugumleri gezme
    }
}

int sizeOfList(){   // null gorene kadar yani tum dugumleri gezene kadar geziyor ve dugum sayisini donduruyor
    node *tmp =head;
    int counter =0;
    while(tmp !=NULL){
        counter++;
        tmp = tmp ->next;
    }
    return counter;  //dugum sayisi donduruluyor
}

void delete(){  //silme islemi demek ki en sonda ki dugum silinecek
    node *tmp = head;
    int flag =1;
    while(flag ==1){
        tmp = tmp->next;    // en sona gelene kadar gidiyoruz
        if(tmp->next == NULL){      //en son dugume geldik
            tmp->prev->next =NULL;  //onceki dugumun nextini null yaptik yani artık sonuncu oluyor
            free(tmp);      //hafiza serbest birakildi
            flag = 0;       //donguden cik
        }
    }
}

void clearList() {
    node *tmp = head; // baslangic atamasi
    while(tmp != NULL) {
        head = head->next;      //head sonra ki dugume geciyor
        free(tmp->variable); // Bellekten veriyi temizle
        free(tmp); // dugumu sil
        tmp = head; // sonraki dugume gec
    }
}
