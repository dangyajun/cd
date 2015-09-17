call wb_single.bat
D:\app\wkhtmltopdf\bin\wkhtmltopdf --disable-smart-shrinking --margin-top 15 --margin-bottom 15 --header-font-size 10 --footer-font-size 10 --footer-spacing 5 --header-spacing 5 --header-left "CD - A 2D Graphics Library" --header-right "[date]" --footer-right "[page]/[topage]" cd.html cd.pdf
