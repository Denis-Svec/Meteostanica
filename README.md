# Náplň cvičenia
- zoznámenie sa s SPI perifériou
- komunikácia s displejom ILI9163


# Konfigurácia SPI

<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_8/blob/master/images/spi_config.PNG" width="750">
</p>

- Mode - pri komunikácii MCU <=> displej je MCU "master" a displej je "slave"
- Hardware NSS Signal - vypnuté, "slave select" je riadený nami
- Prescaler - nastavuje frekvenciu hodín pre SPI perifériu, od toho závysí "baud rate" (SPI1 je pripojené k APB2 zbernici, ktorá má "clock" s frekvenciou 8MHz, ak pouzijeme prescaler s hodnotou 8 tak výsledná prenosová rýchlosť bude 1MBit/s)
- posielané dáta majú dĺžku 8bit
- Clock polarity - polarita "Low" - pri každom tiku hodím sa úroveň signálu zmení z "Low" na "High" a ak nie je pripojený zdroj hodin, tak je na vodiči úroveň "Low"
- Clock phase - kedy začne vzorkovanie dát a ich posúvanie

- pre 8 bitový prenos, je potrebné doplniť do vygenerovaného kódu nastavenie "FRXTH" v "CR2" registri :
```php
SPI1->CR2 |= 1 << 12;
```

# Displej

- na cvičení sa bude používať displej ILI9163

- knižnica pre tento displej sa nachádza v projekte v priečinku "display"

- vďaka knižnici už nie je potrebné konfigurovať samotný displej, stačí už len využívať funkcie, ktoré poskytuje a vykreslovať text, geometrické útvary, obrázky ... 

<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_8/blob/master/images/displej.jpg" width="500">
</p>

### Pripojenie displeju

- pin LED - pripojiť k napájaniu 3v3
- SCK - pripojiť k PA5 pinu (A4)
- SDA - pripojiť k PA7 pinu (A6)
- A0 - pripojiť k PB6 pinu (D5)
- RESET - pripojiť k PA3 (A2)
- CS - pripojiť k PB5 pinu (D11)
- GND -  kam asi?  ¯\ _(ツ) _/¯
- Vcc - pripojiť na 3v3

# Zadanie
- z PC si cez USART pošlite dáta do MCU a tie následne vykreslite/vypíšte na displej
