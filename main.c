#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

typedef struct Hasta {
    int hastaNo;
    char hastaAdi[30];
    int hastaYasi;
    char cinsiyet[6];
    int mahkumlukDurumBilgisi;
    int engellilikOrani;
    char kanamaliHastaDurumBilgisi[15];
    char hastaKayitSaati[6];
    double muayeneSaati;
    int muayeneSuresi;
    int oncelikPuani;
} Hasta;

typedef struct Heap {
    Hasta* heapArray;
    int maxSize;
    int currentSize;
    int kayitSayisi;
} Heap;

char saat[6];
char muayeneSaati[6] = "09.00";
int muayenehastasayisi = 0;
Hasta* hastaListesi = NULL;
Hasta* siraliHastaListesi = NULL;
int hastaSayisi = 0;
int hastaListesiBoyut = 0;

void initializeHeap(Heap* heap, int initialSize) {
    heap->maxSize = initialSize;
    heap->currentSize = 0;
    heap->heapArray = (Hasta*)malloc(initialSize * sizeof(Hasta));
    heap->kayitSayisi = 0;
}

void destroyHeap(Heap* heap) {
    free(heap->heapArray);
}

void resizeHeap(Heap* heap) {
    heap->maxSize *= 2;
    heap->heapArray = (Hasta*)realloc(heap->heapArray, heap->maxSize * sizeof(Hasta));
}

void insertToHeap(Heap* heap, Hasta item) {
    if (heap->currentSize == heap->maxSize) {
        resizeHeap(heap);
    }
    heap->kayitSayisi++;
    int currentIndex = heap->currentSize;
    heap->heapArray[currentIndex] = item;

    // Yeni eklenen eleman�n �ncelik puan�n� kontrol ederek heap'i yeniden d�zenle
    while (currentIndex > 0 && heap->heapArray[currentIndex].oncelikPuani > heap->heapArray[(currentIndex - 1) / 2].oncelikPuani) {
        Hasta temp = heap->heapArray[currentIndex];
        heap->heapArray[currentIndex] = heap->heapArray[(currentIndex - 1) / 2];
        heap->heapArray[(currentIndex - 1) / 2] = temp;
        currentIndex = (currentIndex - 1) / 2;
    }

    heap->currentSize++;
}

Hasta removeFromHeap(Heap* heap) {
    if (heap->currentSize == 0) {
        printf("Heap bo�. herhangi bir item ��kart�lamaz.\n");
        Hasta emptyHasta = {0};
        return emptyHasta;
    }

    Hasta root = heap->heapArray[0];
    heap->currentSize--;
    heap->kayitSayisi--;

    heap->heapArray[0] = heap->heapArray[heap->currentSize];

    // K�k elemandan ba�layarak heap'i yeniden d�zenle
    int currentIndex = 0;
    while (true) {
        int solCocuk = 2 * currentIndex + 1;
        int sagCocuk = 2 * currentIndex + 2;
        int enBuyuk = currentIndex;

        if (solCocuk < heap->currentSize && heap->heapArray[solCocuk].oncelikPuani > heap->heapArray[enBuyuk].oncelikPuani) {
            enBuyuk = solCocuk;
        }

        if (sagCocuk < heap->currentSize && heap->heapArray[sagCocuk].oncelikPuani > heap->heapArray[enBuyuk].oncelikPuani) {
            enBuyuk = sagCocuk;
        }

        if (enBuyuk != currentIndex) {
            Hasta temp = heap->heapArray[currentIndex];
            heap->heapArray[currentIndex] = heap->heapArray[enBuyuk];
            heap->heapArray[enBuyuk] = temp;
            currentIndex = enBuyuk;
        } else {
            break;
        }
    }

    return root;
}

void initializeHastaListesi(int initialSize) {
    hastaListesiBoyut = initialSize;
    hastaListesi = (Hasta*)malloc(initialSize * sizeof(Hasta));
    siraliHastaListesi = (Hasta*)malloc(initialSize * sizeof(Hasta));
}

void resizeHastaListesi() {
    hastaListesiBoyut *= 2;
    hastaListesi = (Hasta*)realloc(hastaListesi, hastaListesiBoyut * sizeof(Hasta));
    siraliHastaListesi = (Hasta*)realloc(siraliHastaListesi, hastaListesiBoyut * sizeof(Hasta));
}

void hastaKayitOku(const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        printf("Dosya a��lamad� veya Dosya Bo�");
        return;
    }

    char satir[100];
    while (fgets(satir, sizeof(satir), dosya)) {
        if (hastaSayisi == hastaListesiBoyut) {
            resizeHastaListesi();
        }

        char* token = strtok(satir, ",");

        hastaListesi[hastaSayisi].hastaNo = atoi(token);
        token = strtok(NULL, ",");

        strcpy(hastaListesi[hastaSayisi].hastaAdi, token);
        token = strtok(NULL, ",");

        hastaListesi[hastaSayisi].hastaYasi = atoi(token);
        token = strtok(NULL, ",");

        strcpy(hastaListesi[hastaSayisi].cinsiyet, token);
        token = strtok(NULL, ",");

        hastaListesi[hastaSayisi].mahkumlukDurumBilgisi = (strcmp(token, "true ") == 0) ? 1 : 0;
        token = strtok(NULL, ",");

        hastaListesi[hastaSayisi].engellilikOrani = atoi(token);
        token = strtok(NULL, ",");

        strcpy(hastaListesi[hastaSayisi].kanamaliHastaDurumBilgisi, token);
        token = strtok(NULL, ",");

        strcpy(hastaListesi[hastaSayisi].hastaKayitSaati, token);
        token = strtok(NULL, ",");

        hastaListesi[hastaSayisi].muayeneSaati = atof(token);
        token = strtok(NULL, ",");

        hastaListesi[hastaSayisi].muayeneSuresi = atoi(token);
        token = strtok(NULL, ",");

        hastaListesi[hastaSayisi].oncelikPuani = oncelikPuaniHesapla(&hastaListesi[hastaSayisi]);
        hastaListesi[hastaSayisi].muayeneSuresi = muayeneSuresiHesapla(&hastaListesi[hastaSayisi]);

        hastaSayisi++;
    }

    fclose(dosya);
}

void hastaBilgileriniYazdir(const Hasta* hastaListesi, int hastaSayisi) {
    int i;
    for (i = 0; i < hastaSayisi; i++) {
        printf("Hasta No: %d\n", hastaListesi[i].hastaNo);
        printf("Hasta Ad�: %s\n", hastaListesi[i].hastaAdi);
        printf("Hasta Ya��: %d\n", hastaListesi[i].hastaYasi);
        printf("Cinsiyet: %s\n", hastaListesi[i].cinsiyet);
        printf("Mahkumluk Durumu: %s\n", hastaListesi[i].mahkumlukDurumBilgisi ? "Evet" : "Hay�r");
        printf("Engellilik Oran�: %d\n", hastaListesi[i].engellilikOrani);
        printf("Kanamal� Hasta Durumu: %s\n", hastaListesi[i].kanamaliHastaDurumBilgisi);
        printf("Hasta Kay�t Saati: %s\n", hastaListesi[i].hastaKayitSaati);
        printf("Muayene Saati: %.2f\n", hastaListesi[i].muayeneSaati);
        printf("Muayene S�resi: %d dakika\n", hastaListesi[i].muayeneSuresi);
        printf("�ncelik Puan�: %d\n", hastaListesi[i].oncelikPuani);
        printf("------------------------------------\n");
    }
}

void hastaListesiniKayitSaatiSiralama(const Hasta* hastaListesi, int hastaSayisi, Hasta* siraliHastaListesi) {
    // Hasta listesini kay�t saatine g�re s�ralama
    int i;
    for (i = 0; i < hastaSayisi; i++) {
        siraliHastaListesi[i] = hastaListesi[i];
    }

    for (i = 0; i < hastaSayisi - 1; i++) {
        int j;
        for (j = 0; j < hastaSayisi - i - 1; j++) {
            if (strcmp(siraliHastaListesi[j].hastaKayitSaati, siraliHastaListesi[j + 1].hastaKayitSaati) > 0) {
                Hasta temp = siraliHastaListesi[j];
                siraliHastaListesi[j] = siraliHastaListesi[j + 1];
                siraliHastaListesi[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < hastaSayisi - 1; i++) {
        siraliHastaListesi[i].hastaNo = (i + 1);
    }
}

int oncelikPuaniHesapla(const Hasta* hasta) {
    int oncelikPuani = 0;

    // Ya�a g�re puan ekleme
    if (hasta->hastaYasi >= 0 && hasta->hastaYasi <= 5) {
        oncelikPuani += 20;
    } else if (hasta->hastaYasi > 5 && hasta->hastaYasi <= 45) {
        oncelikPuani += 0;
    } else if (hasta->hastaYasi > 45 && hasta->hastaYasi <= 65) {
        oncelikPuani += 15;
    } else if (hasta->hastaYasi > 65) {
        oncelikPuani += 25;
    }

    // Mahkumluk durumuna g�re puan ekleme
    if (hasta->mahkumlukDurumBilgisi == 1) {
        oncelikPuani += 50;
    }

    // Engellilik oran�na g�re puan ekleme
    oncelikPuani += (hasta->engellilikOrani) / 4;

    // Kanamal� hasta durumuna g�re puan ekleme
    if (strcmp(hasta->kanamaliHastaDurumBilgisi, "kanama    ") == 0) {
        oncelikPuani += 20;
    } else if (strcmp(hasta->kanamaliHastaDurumBilgisi, "agirKanama") == 0) {
        oncelikPuani += 50;
    }

    return oncelikPuani;
}

int muayeneSuresiHesapla(const Hasta* hasta) {
    int muayeneSuresi = 10;  // Standart muayene s�resi

    // Hasta ya�� 65'ten k���kse 0 dakika eklenir, aksi takdirde 15 dakika eklenir
    if (hasta->hastaYasi < 65) {
        muayeneSuresi += 0;
    } else {
        muayeneSuresi += 15;
    }

    // Engellilik oran� 5'e b�l�nerek dakika olarak eklenir
    muayeneSuresi += (hasta->engellilikOrani) / 5;

    // Kanamal� hasta durumu kontrol edilir ve buna g�re dakika eklenir
    if (strcmp(hasta->kanamaliHastaDurumBilgisi, "kanama    ") == 0) {
        muayeneSuresi += 10;
    } else if (strcmp(hasta->kanamaliHastaDurumBilgisi, "agirKanama") == 0) {
        muayeneSuresi += 20;
    }

    return muayeneSuresi;
}

///hasta kay�t ekleme b�l�m�

void saatEkle(char *saat, int dakika) {
    int saat_saat, saat_dakika;
    sscanf(saat, "%d.%d", &saat_saat, &saat_dakika);

    saat_dakika += dakika;
    saat_saat += saat_dakika / 60;
    saat_saat = saat_saat % 24;
    saat_dakika %= 60;

    sprintf(saat, "%02d.%02d", saat_saat, saat_dakika);
}

void HastaMuayeneEt(struct Hasta MuayeneHastasi) {
    printf("\n%d. s�rada %d numaral� %s adl� hasta muayene olmu�tur.\n", muayenehastasayisi, MuayeneHastasi.hastaNo, MuayeneHastasi.hastaAdi);
    printf("�ncelik Puan�: %d   Muayene s�resi: %d  Muayene Giri� Saati:%s\n", MuayeneHastasi.oncelikPuani, MuayeneHastasi.muayeneSuresi, muayeneSaati);
    saatEkle(muayeneSaati, MuayeneHastasi.muayeneSuresi);
}

int saatKarsilastir(char saat1[], char saat2[]) {
    int saat1_saat, saat1_dakika;
    int saat2_saat, saat2_dakika;

    sscanf(saat1, "%d.%d", &saat1_saat, &saat1_dakika);
    sscanf(saat2, "%d.%d", &saat2_saat, &saat2_dakika);

    if (saat1_saat > saat2_saat || (saat1_saat == saat2_saat && saat1_dakika > saat2_dakika)) {
        return 1;
    } else {
        return 0;
    }
}

void HastaKayitEkle() {
    Heap heap;
    initializeHeap(&heap, 10); // Ba�lang�� boyutunu 10 olarak belirlendi�
    int i = 0;

    while (i < hastaSayisi) {
        if (heap.kayitSayisi == 0) {
            siraliHastaListesi[i].oncelikPuani = oncelikPuaniHesapla(&siraliHastaListesi[i]);
            insertToHeap(&heap, siraliHastaListesi[i]);
            strcpy(saat, siraliHastaListesi[i].hastaKayitSaati);
            i++;
        } else if (saatKarsilastir(siraliHastaListesi[i].hastaKayitSaati, muayeneSaati)) {
            muayenehastasayisi++;
            HastaMuayeneEt(removeFromHeap(&heap));
        } else {
            insertToHeap(&heap, siraliHastaListesi[i]);
            strcpy(saat, siraliHastaListesi[i].hastaKayitSaati);
            i++;
        }
    }
    while (heap.kayitSayisi != 0) {
        muayenehastasayisi++;
        HastaMuayeneEt(removeFromHeap(&heap));
    }
    destroyHeap(&heap);
    printf("\n\n T�M HASTALAR BA�ARIYLA TEDAV� ED�LD�...:)\n");
}

int main() {
    setlocale(LC_ALL, "Turkish");
    const char* dosyaAdi = "hasta.txt";

    initializeHastaListesi(10); // Ba�lang�� boyutunu 10 olarak belirledik
    hastaKayitOku(dosyaAdi);
    hastaListesiniKayitSaatiSiralama(hastaListesi, hastaSayisi, siraliHastaListesi);
    hastaBilgileriniYazdir(siraliHastaListesi, hastaSayisi);
    
    HastaKayitEkle();
    
    free(hastaListesi);
    free(siraliHastaListesi);
    
    return 0;
}

