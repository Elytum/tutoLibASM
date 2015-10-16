Pré-scriptum: Il se peut que je raconte des conneries, le tuto est fait à l'arrache.

Un peu de documentation utile:

	#####################################      LIRE        #####################################
	# Un très bon tuto général et complet: http://benoit-m.developpez.com/assembleur/tutoriel/ #
	#####################################     VRAIMENT     #####################################

	https://software.intel.com/en-us/articles/introduction-to-x64-assembly
	Liste des registres: http://warppipe.net/blog/?page_id=349
	Syscalls: http://asm.sourceforge.net/syscall.html
	Liste des instructions: http://faydoc.tripod.com/cpu/index_b.htm

Alors, l'asm c'est quoi ?
En gros le C est convertis en assembleur pour être ensuite convertis en binaire puis exécuté par le processeur.
Nous allons donc descendre d'une couche et quitter le C pour passer à l'asm.

Prérequis:
	Installer "nasm" à l'aide de brew.
	gcc, déjà ici donc c'est cool.

Pour y aller calmement, tentons une traduction d'un code en C simple:

ft_is_A :

int		ft_is_A(char c)
{
	return (c == A);
}

Bon ok, il est un peu compressé, on va devoir le rendre un peu plus explicit pour simplifier la transformation en asm:

int		ft_is_A(char c)
{
	if (c == 'A')
		return (1);
	return (0);
}

Nous avons donc trois choses:
	-Une comparaison
	-Si la comparaison renvoie TRUE:
		Un ret 1
	-Un ret 0

Les instructions requises sont donc:

mov: http://faydoc.tripod.com/cpu/mov.htm
cmp: http://faydoc.tripod.com/cpu/cmp.htm
ret: http://faydoc.tripod.com/cpu/ret.htm

je: http://faydoc.tripod.com/cpu/je.htm

mov, cmp, ret sont implicits dans dans le résumé ci-dessus, mais pourquoi un je ?
Parce qu'en vrai, une "fonction" n'existe pas, c'est un block de code binaire (et donc ici aussi en assembleur)
avec ce qu'on appel un label, et le je permet de sauter au debut d'un label et de revenir à l'instruction suivante
une fois l'appel à la "fonction" fait. Heureusement, en assembleur, il est aussi possible de créer des "fonctions",
c'est ce qu'on appel un "label".

Résumons donc en pseudo code :

ft_is_A:
	cmp <FIRST_ARGUMENT> 'A'
		ret 0
	ret 1

Mais pourquoi le jump ?
Et comment return une value alors que ret ne prend pas d'argument ? C'est simple, il existe un registre, rax,
qui a pour but de contenir la valeur de retour des fonctions, c'est une norme, on peut utiliser n'importe quel
registre, mais dans ce cas, comment savoir dans lequel aller chercher le retour ? D'où cette convention.

On mov donc la valeur qu'on veut return dans rax avant de ret, et le ret stoppera la fonction.
on remplace donc:

ret 0
			en
mov rax, 0
ret

ret 1
			en
mov rax, 1
ret

Ensuite, un deuxième problème, le cmp ne permet que de modifier un flag qui peut être utilisé dans un jump,
alors comment exécuter deux instructions avec un jump ? Avec un label !

ft_is_A:
	cmp <FIRST_ARGUMENT> 'A'
		ret 0
	ret 1
							devient
do_ret1:
	mov rax, 1
	ret

ft_is_A:
	cmp <FIRST_ARGUMENT> 'A'
		je do_ret1
	mov rax, 0
	ret

Il ne manque plus qu'à remplacer notre <FIRST_ARGUMENT> par le bon registre.
Comme on peut le voir dans "http://warppipe.net/blog/?page_id=349",
rdi est le registre correspondant au premier argument
On veut un char, on ne prend donc que les 8 derniers bits, ce qui correspond à sil
Le code final donne donc:

section .text
	global _ft_is_A

_ft_is_A:
	cmp sil, 'A'
		je do_ret1
	mov rax, 0
	ret

do_ret1:
	mov rax, 1
	ret

("
section .text
	global _ft_is_A
"
permet de signaler le nom de la fonction)

"Et voilà"

Il suffit ensuite de créer un main pour le compiler avec le prototype indiqué et go:
	nasm -f macho64 ft_is_a.s && gcc -c main.c && gcc ft_is_a.o main.o

Maintenant, essayez un "ft_is_alpha(int c)" en assembleur ! Bonne chance !


























































































Un bzero d'exemple proposé par Benois:

global _ft_bzero

section .text

_ft_bzero:
        ; je sauveguarde les 2 parametre de la fonction dans la stack
	push rdi
	push rsi
        ; j'initiallise rax a 0 et rcx (registre compteur par convention) a la valeur du 2eme parametre
	xor rax, rax   ; = mov rax, 0
	mov rcx, rsi
lp:
	cmp rcx, 0x0    ; je verifi si rcx = 0
	je end          ; si rcx = 0, je jump au label end
	stosb           ; stosb set la valeur pointer par rdi par la valeur de rax et incremente rdi
	dec rcx         ; je decremente rcx
	jmp lp          ; jump au label lp
end:
        ; je restore les 2 parametre avant de return (rax est ignore la fonction retourne void)
	pop rsi
	pop rdi
	ret
