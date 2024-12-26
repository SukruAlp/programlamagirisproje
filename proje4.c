#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define KOTA_LIMITI_MB 6144   // 6 GB kota
#define KOTA_ASIM_UCRET 0.011719 // Kota a��m �creti (TL/MB)

void hesaplaVeYaz(char *ay, FILE *adslFile, FILE *faturaFile) {
    char line[MAX_LINE_LENGTH];
    int toplamIndirmeMB = 0, toplamYuklemeMB = 0;
    float kotaAsimi = 0.0;
	float kotaAsimiUcreti = 0.0;

    while (fgets(line, sizeof(line), adslFile) != NULL) {
        int ayNumarasi, gun, indirmeMB, yuklemeMB;

        // Sat�r� par�alara ay�rarak i�le
        sscanf(line, "%d %d %d %d", &ayNumarasi, &gun, &yuklemeMB, &indirmeMB);

        // Ay bilgisi e�le�iyorsa, ilgili verileri topla
        if (ayNumarasi == atoi(ay)) {
            toplamIndirmeMB += indirmeMB;
            toplamYuklemeMB += yuklemeMB;
        }
    }
    


    // Kota a��m� hesaplama
    if (toplamIndirmeMB > KOTA_LIMITI_MB) {
        kotaAsimi = toplamIndirmeMB - (KOTA_LIMITI_MB);
        kotaAsimiUcreti = kotaAsimi * KOTA_ASIM_UCRET;
    } else {
        kotaAsimiUcreti = 0.0;
    }

    // Fatura bilgilerini yaz
    fprintf(faturaFile, "Ay: %s\n", ay);
    fprintf(faturaFile, "Toplam Indirme: %.2f GB\n", toplamIndirmeMB /1024.0);
    fprintf(faturaFile, "Toplam Yukleme: %.2f GB\n", toplamYuklemeMB /1024.0);
    fprintf(faturaFile, "Kota Asimi: %.2f MB\n", kotaAsimi);
    fprintf(faturaFile, "Kota Asim Ucreti: %.4f TL\n", kotaAsimiUcreti);

}

int main() {
    FILE *adslFile = fopen("adsl.txt", "r");
    if (adslFile == NULL) {
        printf("adsl.txt dosyasi acilamadi.\n");
        return 1;
    }

    FILE *faturaFile = fopen("fatura.txt", "w");
    if (faturaFile == NULL) {
        printf("fatura.txt dosyasi acilamadi.\n");
        fclose(adslFile);
        return 1;
    }

    char ay[3]; // Kullan�c�dan al�nacak ay bilgisi (�rne�in: "01" Ocak i�in)
    printf("Lutfen faturasi hesaplanacak ay numarasini girin (01-12): ");
    scanf("%s", ay);

    // Dosyadaki verileri i�le ve faturay� yaz
    hesaplaVeYaz(ay, adslFile, faturaFile);

    // Dosyalar� kapat
    fclose(adslFile);
    fclose(faturaFile);

    printf("Fatura %s ayi icin fatura.txt dosyasina yazildi.\n", ay);
    return 0;
}

