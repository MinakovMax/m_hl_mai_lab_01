# m_hl_mai_lab_01
Лабораторная работа по архитектуре ИС 

# Архитектура проекта в нотации C4
![Diagram](http://www.plantuml.com/plantuml/png/fPJBJjjC5CVtVehveazg8Y3UT8KK2GXPL8AAag0k8ySU4gjZEx8dXAYgHCAbgb8fxRQRqaNt8JJ2a0klCFD6FSSOEmcnAWGIYkSo__zlpcKsFAwvl64nvJ_Jrbd3eAJ6UTrRLrLNQsQh9gyrAWsFkhfZSshphEvOQfrfDfvPyxZ1p8gQVxssXrC7kpkgfNcSkZ1LpiC9pRIfcwsZlh9f60L6BH0fQTLqoZ9rrobIzyZKQMgL_E_Lj3hD5SngZHTDWnf57ASpi5AfvXtckBbKtd7jG-OqgRl56i6XG_085viramTpZ5QfRPJeCSyzc7YXIMOHmUEEgrLdtFUTXWtMMyxn4dSMe7re9Nc3lh97NS-nqvfXcVOgIOblueyO2rzCX2_RiYDFH5zsn5IUmEPmBmONj_uKKp6INyKD_0w2RKCSesgn1J6roeLZdcxIIja3CpZndbRMP5lqnQti4F0PoHv10t4bfc3L9R9Dv2dOJsHNZ77750RmdmeF8nVQo1Dv9eOWqvunsy9GQ6uh3Oa_f2wrTVfoumpvg112NaMbCNiVyKEC06c8K21_1z-tH0nWuYgoG_GCIiGAEZCXd-KmoQZp2wy41ttWW6E3XuY3t0s4zG9-VPJ9hwoa5dS3zJtU18OZ18797X7tIpFJmyHY7N2jwf4ScI_e386_UT0ZqpEJEGV81vyTD4dYmp0YLNIZYRWMVJWips41upmWmiTnDvv9Pbm0XBJNa_Zmqk1-ZrCO0daALTDzBgUF-PZ0_-WX6ycooL6TOzwkf8qAkdy7KHI4AwF310E0MBkjuhkTWEytEi1Ia5OndDj9n3H6yMCKs8RGM06o3OnV7ei2n6F6w1q9AbFvf2ZAFcLXOyS57hOuw7wE8R05pu8WZH-BSnreefJ2vBcA-Oo7i1Rsr6h4VGUg20VTTeC1ZYpyX1272c5I86THgd088cCKW8lsadAKmKk6BztSBIzX-_NCUtU10LCnXZWfxbHv6iH-54TXQOVXavG5cy0dDdZQxvUFJmyMLFjV_PfYZgwnckDnjHwymE5D5dM3kdhpjbZOBwd5mauXNr8FzhOtImNKWA9wIeODdWmsr-Fzfp31wP05lr-4WgsSJBAyrgVYTfu8HK8W_7mUpmQrZOR5_W80)
## Список компонентов  

### Сервис авторизации
**API**:
-	Создание нового пользователя +
      - входные параметры: login, пароль, имя, фамилия, email, обращение (г-н/г-жа)
      - выходные параметры: отсутствуют
-	Поиск пользователя по логину +
     - входные параметры:  login
     - выходные параметры: имя, фамилия, email, обращение (г-н/г-жа)
-	Поиск пользователя по маске имени и фамилии +
     - входные параметры: маска фамилии, маска имени
     - выходные параметры: login, имя, фамилия, email, обращение (г-н/г-жа)

### Сервис визитов
**API**:
- Фиксация визита +
  - Входные параметры: ID пользователя, время визита
  - Выходыне параметры: идентификатор визита
- Получение списка всех визитов +
  - Входные параметры: отсутствуют
  - Выходные параметры: время визита, данные пользователя

### Сервис темпирауры
**API**:
- Измерение текущего состояния темпиратуры +
  - Входные параметры: данные с датчика темпиратуры, время измерения, id пользователя
  - Выходные параметры: идентификатор измерения
- Получение данных о температуре
  - Входные параметры: интересующий диапазон даты и времени
  - Выходные параметры: массив с измерениями темпиратуры

# Модель данных
![Diagram](http://www.plantuml.com/plantuml/png/RKv13W912Blp2ZxW5ym-GhrkY5iD2OncOBmO_-wuPdLZlD02JTiXKcesDwAZIGGVD3JlnAnJ7orGnvbDaaXrq6DHxlkFFqgxdBLqF6cD78iuNaVvShYeTKnDMxdiu5TKoLR1Vs9lOWtCFmNcwCtsNNRPrbP40yhKt9u0)
