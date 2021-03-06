# Corba-remote-server

Данная клиент-серверная программа предназначена для демонстрации коммуникаций между распределенными приложениями по стандарту CORBA, а также для демонстрации взаимодействии классов Qt и omniORB. Клиентская часть при каждом нажатии на клавиатуру отправляет печатаемый символ на серверную часть, где он сразу и отображается. В проекте используются библиотеки (программные компоненты) следующих версий:__

- omnniORB версии 4.2.2 (http://omniorb.sourceforge.net)
- ncurses версии 6.1 (https://www.gnu.org/software/ncurses/ncurses.html)

Проект написан в среде Qt Creator. Используется система сборки qmake. Проект состоит из двух подпроектов - клиентской и серверной части.

Для считывания нажатий клавиши и мгновенной обработки после нажатия задействованы библиотеки conio для Windows (поставляется штатно) и ncurses для Linux (сторонняя библиотека,  загружается с сайта или с репозитория дистрибутива). При разработке учитывалось возможное отличие кодировок у серверного и клиентского приложения, поэтому при печате происходит перекодировка. Для Windows ожидается кодировка CP-1251 (необходимо переключить через chcp 1251, если старые версии Windows), для Linux - UTF-8. В репозитории представлена Linux-версия проекта (для Windows необходимо изменить инструкции #include, если не используется Cygwin c MinGW).

## Инструкция
- Установите omniORB (следуйте инструкциям с сайта http://omniorb.sourceforge.net и с файлов README* в проекте с omniORB).
- Для компиляции под Linux установите библиотеки ncurses с поддержкой широких символов. Рекомендую через пакетный менеджер с официального репозитория - мне встречались жалобы, что при сборке последней версии с официального сайта появлялись проблемы с некоторыми предустановленными консольными приложениями. Для debian-производных дистрибутивов устанавливаемые пакеты называются ncurses-base, libncursesw5, libncursesw5-dev
- Запустите omniNames с параметрами -start <port>, где  нужно указать порт. Например,
```console  
omniNames -start 2000 
```
(для Linux, в Windows - omniNames.exe).
- Возможно понадобится перекомпиляция интерфейсного файла remote-print.idl (особенно если используется отличающаяся версияя omniORB). Для этого введите в корневом каталоге проекта 
```console
  omniidl -bcxx remote-print.idl.
```
- Запустите серверную часть проекта corba-output-server с указанием параметров -ORBInitRef NameService=corbaloc:iiop:<ip-addr\>:<port\>/NameService , где вместо <ip-addr\> нужно указать IP адрес, а вместо <port\> указать порт. Например, 
```console 
./corba-output-server -ORBInitRef NameService=corbaloc:iiop:127.0.0.1:2000/NameService 
```
(для Linux, в Windows - corba-output-server.exe).
- Запустите клиентскую часть, проекта corba-output-client с указанием параметров -ORBInitRef NameService=corbaloc:iiop:<ip-addr\>:<port\>/NameService , где вместо <ip-addr\> нужно указать IP адрес, а вместо <port\> указать порт. Например, 
```console
./corba-output-client -ORBInitRef NameService=corbaloc:iiop:127.0.0.1:2000/NameService 
```
(для Linux, в Windows - corba-output-client.exe)
- Введите символы в клиентском приложении. Они должны моментально отображаться в сервеном приложении.
