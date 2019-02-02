/* testandset.S - Contains the function test_and_set */

		.text
		.global	test_and_set
		
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*
*	test_and_set(uint32 *,uint32) - implementation of the function test_and_set. Call is test_and_set(uint32 *, uint32)
*
/-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

test_and_set:
			//pushl 	%ebp
			//movl	%esp,  %ebp
			//pushl	%esi
			//pushl	%edi
			//pushl	%ebx
			//sub	 	$4,%esp
			//movl	8(%ebp),%edx
			//movl 	%edx, -16(%ebp)
		       //	movl	12(%ebp),%eax
			//xchgl	(%edx),%eax
			//add		$4,%esp
			//popl		%ebx
			//popl		%edi
			//popl		%esi
			//movl	%ebp,%esp
			//popl		%ebp
			movl	4(%esp), %edx
			movl	8(%esp), %eax
			xchg	(%edx), %eax
			ret
