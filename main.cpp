  ///////   //     //   
 //     //  //    //  
 //     //  //   //   
 //     //  //  //    
 //     //  //   //     
 //     //  //     //     
  ///////   //      //
#include <WiFiServer.h>
#include <WiFi.h>

WiFiServer sv(5000); //Cria o objeto servidor na porta 5000
WiFiClient cl;       //Cria o objeto cliente.

int byteToInt(byte byteValue) {
    return (int)byteValue;
}

byte intToByte(int8_t intValue) {
    return (byte)intValue;
}

void TCP()
{
    if (cl.connected()) //Detecta se há clientes conectados no servidor
    {
        if (cl.available() > 0) //Verifica se o cliente conectado tem dados para serem lidos
        {
        digitalWrite(GPIO_NUM_2, 1);
        byte received;
        received = cl.read();
        float erro = (float)byteToInt(received);
        if (erro>127){erro = 0;} //for negative numbers
        float Kp = 5;
        float f = (erro) * Kp; //multiply by Ke
        f = f * f; //fˆ2
        int u = f; 
        if(u>100){u = 100;} //ensure the values for u E [0:100]
        if(u<=0){u = 0.00001;}
        Serial.print("u:");
        Serial.println(u);
        byte byteToSend = intToByte(u);
        cl.write(byteToSend);
        }
    }
    else //Se nao houver cliente conectado
    {
        digitalWrite(GPIO_NUM_2, !digitalRead(GPIO_NUM_2));
        cl = sv.available(); //Disponabiliza o servidor para o cliente se conectar
        delay(100);
    }
}

void setup()
{
    pinMode(GPIO_NUM_2, OUTPUT);
    Serial.begin(115200); //Habilita a comm serial
    WiFi.mode(WIFI_AP);   //Define o WiFi como Acess_Point
    WiFi.softAP("NodeMCU", ""); //Cria a rede de Acess_Point
    sv.begin(); //Inicia o servidor TCP na porta declarada no começo (5000)
}

void loop()
{
    TCP(); //Funçao que gerencia os pacotes e clientes TCP.
}
