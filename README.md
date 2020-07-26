# Health System Component

Гибкая система здоровья и брони которую можно подключить к любому Actor в том числе и персонажу. Система создана на **Unreal Engine 4.25+**

## Особенности
- Гибко настраиваемая система регенерации для здоровья и брони.
- Вы можете сразу же начать использовать его в своем проекте не внося никаких изменений.

## Установка
- Клонируйте репозиторий в любую удобную папку.
- Переместите папку HealthSystem в папку "Plugins" вашего проекта.
- Добавьте в ваш Actor компонент **HealthComponent** он находится в категории **Custom**:
![image](https://github.com/Netlex/HealthSystem/raw/master/Resources/AddHealthComponent.PNG)
- Если система подключена к персонажу то вы можете добавить компонент **ControllerEvents** в PlayerController для обработки событий системы здоровья:
![image](https://github.com/Netlex/HealthSystem/raw/master/Resources/AddControllerEvents.PNG)
- Для дополнительной проверки на урон и его изменение вы можете подключить интерфейс **DamageHealth** который содержит 2 функции, CanApplyDamage является дополнительной проверкой на нанесение урона, а с помощью ModifireDamage вы можете изменять наносимый урон. 
![image](https://github.com/Netlex/HealthSystem/raw/master/Resources/AddDamageHealth.PNG) ![image](https://github.com/Netlex/HealthSystem/raw/master/Resources/FunctionsDamageHealth.PNG)
