
Calculer un pas de tps et un pas de puissance

1. tester si on est au bon [date] timestamp
2. tant que le timestamp courant est inférieur à durée de montée, on incrémente la puissance à chaque pas de tpsi
2 bis tant que la puissance est inférieure à 255, je 'incrémente la puissance tous les pas de tps calculés
3. Delay de 1 minute à puissance 255
4. Decrémenter de pas de puissance jusqu'à puissance = 0

Incrémenter index_date

Ajouter la moniteur série 
