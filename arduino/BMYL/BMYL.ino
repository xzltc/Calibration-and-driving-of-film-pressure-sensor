#include <LiquidCrystal_I2C.h>        //调用I2C库文件
LiquidCrystal_I2C lcd(0x27,20,4);     //定义1602参数
int sensorPin = A0;                   //定义模拟量输入引脚
int sensorValue = 0;                  //定义AD采集初始值

void setup() 
{
  Serial.begin(9600);                 //设置波特率
  lcd.init();                         //初始化LCD
  lcd.backlight();                    //设置LCD背景亮
  lcd.setCursor(0,0);                 //设置显示指针   
  lcd.print("Initialize OK");         //输出初始化字符到LCD1602上
  delay(1000);                     
}

void loop() {
  sensorValue = analogRead(sensorPin); //读取A0引脚的模拟值
  double i = 0 ;                       //方法一：定义用来存储1/AD的数据
  double j = 0 ;                       //方法一：存储计算出来的力，带小数，单位为g
  int k = 0;                           //方法一：整数压力值
  double p = 0 ;                       //方法二中间变量
  double q = 0 ;                       //方法二中间变量
  int o = 0;                           //方法二：整数压力值
   /*******以下为方法一*******/
  i = 1 / float(sensorValue);          //计算i值
  j = 20000000*sq(i)+31334*i+11.994;   //计算j值 （此处替换为自己试验标定所得的函数）
  k = int(j);                          //保留j的整数部分存入k中
   /*******以下为方法二*******/
  p = (sensorValue - 990.4)/ -262 ;    //p,q为使用log函数进行标定的中间变量（此处替换为自己试验标定所得的函数）
  q = pow(10,p);
  o = int(q);
    
  /*******以下为串口输出显示部分*******/
  Serial.print("AD值 = ");              //串口输出“AD值 = xxx”
  Serial.println(sensorValue);         
  Serial.print("1/AD = ");              //串口输出“1/AD = xxx”
  Serial.println(i,8);  
  Serial.print("方法一： ");             //串口输出“方法一：xxx g”
  Serial.print(k);
  Serial.println("g");
  Serial.print("方法二： ");             //串口输出“方法二：xxx g”
  Serial.print(o);
  Serial.println("g");
  Serial.println( );
    
  /*****以下为LCD1602输出显示部分******/
  String m = String(k);                //定义m为k值的字符串
  String n = String(o);
  int l = m.length();                  //计算字符串m的长度并存储到l变量中
  int ll = n.length();                 //计算字符串n的长度并存储到ll变量中
  
  lcd.setCursor(0,0);                    //LCD首行显示为“AD value = XXXX”
  lcd.print("AD value = ");
  lcd.setCursor(11,0);    
  lcd.print(sensorValue);
  if (sensorValue < 1000)              //以下的if函数用来刷新屏幕上残留的字符
     {
       lcd.setCursor(14,0);    
       lcd.print(" ");
      }
  if (sensorValue < 100)
     {
       lcd.setCursor(13,0);
       lcd.print("  "); 
     }   
  lcd.setCursor(0,1);                  //LCD第二行显示为“M = XXXX”
  lcd.print("M = ");                   
  lcd.setCursor(4,1);
  lcd.print(k);
  lcd.setCursor(4+l,1);                //"g"刷新程序
  lcd.print("/");                      //之后显示用log10标定的重量值
  lcd.setCursor(5+l,1);                //"g"刷新程序
  lcd.print(o);
  lcd.setCursor(5+l+ll,1);             //"g"刷新程序
  lcd.print(" g   ");
  delay(100);
  }
