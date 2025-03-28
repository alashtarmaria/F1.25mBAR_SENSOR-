char buffer[64];

void UART3_Init_Config(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT,  _U3RX);
    PPS_Mapping(68, _OUTPUT, _U3TX);
    UART3_Init(baudrate);
    Delay_ms(100);
}

void SPI1_PPS_Init() {
    PPS_Mapping(96, _OUTPUT, _SCK1OUT);
    PPS_Mapping(97, _OUTPUT, _SDO1);
    PPS_Mapping(98, _INPUT, _SDI1);
    PPS_Mapping(104, _OUTPUT, _SS1OUT);

    TRISF.F0 = 0;
    TRISF.F1 = 0;
    TRISF.F2 = 1;
    TRISF.F8 = 0;
    LATF.F8 = 1;
}

void SPI1_Initialize_Master() {
    SPI1_Init();
}

unsigned int Read_ADC_Channel(unsigned char channel) {
    unsigned char msb, lsb;
    unsigned int adcValue;
    unsigned char command;
    unsigned char channelBits = channel << 6;

    if (channel > 3)
        command = 0b00000111;
    else
        command = 0b00000110;

    LATF.F8 = 0;
    SPI1_Write(command);
    msb = SPI1_Read(channelBits);
    lsb = SPI1_Read(0x00);
    LATF.F8 = 1;

    msb = msb & 0b00001111;
    adcValue = (msb << 8) | lsb;
    return adcValue;
}

void Read_Pressure() {
    unsigned int adcRaw;
    float voltage, pressure_psi, pressure_bar;
    char txt[128];

    // Ayarlanabilir parametreler
    float Vsup = 5.0;        // Sensör beslemesi
    float Pmin = 0.0;        // Min. basinç (psi)
    float Pmax = 150.0;      // Max. basinç (psi)

    // ADC verisi okunuyor
    adcRaw = Read_ADC_Channel(2);  // CH0
    voltage = (adcRaw / 4095.0) * Vsup;  // ADC -> Voltaj

    // Basinç hesaplama (psi cinsinden)
    pressure_psi = ((voltage - 0.1 * Vsup) / (0.8 * Vsup)) * (Pmax - Pmin) + Pmin;

    // psi -> bar
    pressure_bar = pressure_psi * 0.06895;

    // UART ile yazdir
    sprintf(txt,
        "ADC: %u\r\nVoltage: %.3f V\r\nPressure: %.2f psi / %.3f bar\r\n\r\n",
        adcRaw, voltage, pressure_psi, pressure_bar);

    UART3_Write_Text(txt);
    Delay_ms(1000);
}
void Read_Pressure_160mbar() {
    unsigned int adcRaw;
    float voltage, pressure_mbar;
    char txt[128];

    // Parametreler
    float Vsup = 5.0;
    float Pmin = 0.0;
    float Pmax = 160.0;

    // CH2 okunuyor
    adcRaw = Read_ADC_Channel(3);
    voltage = (adcRaw / 4095.0) * Vsup;

    // Basinç hesaplama (mBar cinsinden)
    pressure_mbar = ((voltage - 0.1 * Vsup) / (0.8 * Vsup)) * (Pmax - Pmin) + Pmin;

    sprintf(txt,
        "ADC: %u\r\nVoltage: %.3f V\r\nPressure: %.2f mBar\r\n\r\n",
        adcRaw, voltage, pressure_mbar);

    UART3_Write_Text(txt);
    Delay_ms(1000);
}

void Read_Pressure_1_25mbar() {
    unsigned int adcRaw;
    float voltage, pressure_pa, pressure_mbar;
    char txt[128];

    float Vsup = 5.0;

    adcRaw = Read_ADC_Channel(4);  // CH4
    voltage = (adcRaw / 4095.0) * Vsup;

    // Diferansiyel basinç hesaplama
    pressure_pa = voltage / 0.019;              // Pa cinsinden
    pressure_mbar = pressure_pa / 100.0;        // mBar cinsinden

    sprintf(txt,
        "ADC: %u\r\nVoltage: %.3f V\r\nPressure: %.2f Pa / %.3f mBar\r\n\r\n",
        adcRaw, voltage, pressure_pa, pressure_mbar);

    UART3_Write_Text(txt);
    Delay_ms(1000);
}


void main() {
    PLLFBD = 70;
    CLKDIV = 0x0000;

    SPI1_PPS_Init();
    SPI1_Initialize_Master();
    UART3_Init_Config(9600);

    UART3_Write_Text("Okuma Basladi...\r\n");

    while (1) {
         Read_Pressure_1_25mbar();
    }
}
