#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define _rgb_u16(r, g, b) ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3)

// 24 bit (8 8 8) color to 16 bit (5 6 5) color
uint16_t rgb_u16(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = 0x0000;

	color = _rgb_u16(r, g, b);
	//color = ((color & 0x00ff) << 8) | ((color & 0xff00) >> 8); // rev16

	return color;
}

// C is very gud language in 2022 yes yes very gud :thumbs_up:
// everything works just fine hahah ues yes XD!
// i miss templates from c++ here
// omg the misery 
// this code is so embarassibng

// TODO: maybe change some macros to functions if it is possible?? idk
// something is wrong 35y4tkob mjioeymhubn9 56h89uh 8u96 956yhb8u056ybu 98h06yb9e7nh 50ue0 56yh987unbeb0 987nmh56yn594876yh en49 5687yhbn9487yhb 56e0 6yhb87n58e7n56yhb0960yb7n89h5yb847n9wqg6tv 784348r ty7s53e4r t86yh734r 587yt6 w4t763y9tg876b675v57v64ww7345t6vgw5tv674g5v g6evbn96 8dtgfs 7689w4s5tebvhg78ybvwer t9gb7n8tver gw987nbwertvg987bnvertg987nbwertv9gb7n8t 9gb78y35 75htg6y834r 5v9876ynbt vbg687b ty78vw73f 5t6vgb89376b 5tgv5tv9376b gv9 7b5tg639 tg6b8739 5tg6b839 5tg6879w6vb3 5tgwvtb9g65ne87rv tws894357v6 nbw534873 tv54b987ng3 5tv897ngbw35tv9gb7n83tv5gbn 987v53 9gb7n8vt359 gb7n8wt539w8n tgb753g97n8vt n789sdg fhc arokwejonfv89re hvusybne65rvnmouiygvw45sh7iouj09a8umhwrfecohmujixhjm9ouigh0uj8m9qawrtm9wu8ooct 8ymhgsenvt 0w54y875try478h5t4w09szreohbn t5y78zr4sihgfe(XUODnw94 5t87yvsan4ctvy 98wupme8rtvyponmpw8tvyonmuw84tvnoymuv8yunenw945n67y0b vser0y98v6ny890se54v06yn89543ev0s98n346y5eayn8904wvtn0a98yw4tvayn890vtan0w98y4tvn9tvy8h 9hujrgnviosen vyu6ote98sun5vhbnvh89y on4otvy8ihevyn85h vn4 9y78ohnq0a3ty98v4 h598uneybuwe56n89u7w0un8943vqeavn890yutq34n098avt0yn89AWN089YVT0VN89 YTVN20Y982034TV98YNTV304Y98N354N859 VYN8CT QWN098YTVN 0Y7984TVQW3098YNVQT0YN89TR 9Y8N700YN89y0un89tv0q98myunw34098metynvn89w70543nvy890w4893e9w834vyn5uvyw3u9n8m0 h89ws5e980hh890w3yh908754 89bw345u0e98nvy nu98mser5 8-9yu890-j6e5it78m,p7890p\.,,;[9\ioup\o]678m\ikj\675vt4\gr\2e\c4f3vt5ghbi06yn458g0y7956v t47w8h69t43bwuyh785ut4griwseob v9nvty90874q394 tvy87w734 5tyvb0986q34 5t0y78hrujgfnkdei784yrftuh3wjieqdcsnkv5q7458y3h 2t09uv2n809q76yvb 4 yq03vb987654tu3vq0 t6987ynb45q03 tvy987unb54 7ty80q9354u78 yu039654tqe 7y85ut43wrhe9zsa 785yuth94wri3ewf 5tdr4^YSGEVZ eF%G^&YUTRHV 9f87ung6yh5tiroe u896i075ytjhgfrekdu89 j054i3wsrqeazd0398ji4r 5uqotawDPru890 4jia#WQOFEDSC 95tyh78uoji43wea 879u0 46yj5ir
typedef struct
{
	int64_t top;

	size_t capacity;
	size_t size;
	size_t elem_size;

	void* elements; // my beloved void*
} stack_t;

#define stack_init(stack, type) \
stack->top = -1; \
stack->size = 0; \
stack->capacity = 1; \
stack->elem_size = sizeof(type); \
stack->elements = malloc(stack->capacity * stack->elem_size);

//#define stack_free(stack) \
//free(stack->elements); \
//free(stack); \
//stack = NULL;

void stack_free(stack_t* stack)
{
	free(stack->elements);
	//free(stack);
}

//#define stack_reserve(stack, new_capacity) \
//void* new_ptr = realloc(stack->elements, new_capacity * stack->elem_size); \
//if (new_ptr != NULL) { stack->elements = new_ptr; stack->capacity = new_capacity; } \

void stack_reserve(stack_t* stack, size_t new_capacity)
{
	void* new_ptr = realloc(stack->elements, new_capacity * stack->elem_size);
	if (new_ptr != NULL) { stack->elements = new_ptr; stack->capacity = new_capacity; }
}

#define stack_push(stack, type, value) \
stack->top++; \
stack->size++; \
if (stack->size > stack->capacity) { stack_reserve(stack, stack->size * 2); } \
*((type*)stack->elements + stack->top) = (type)value;

//#define stack_pop(stack) \
//if (stack->top == -1) { return; } \
//stack->top--; \
//stack->size--; \
//if (stack->size < stack->capacity) { stack_reserve(stack, stack->size); }

void stack_pop(stack_t* stack)
{
	if (stack->top == -1) { return; }

	stack->top--;
	stack->size--;

	if (stack->size < stack->capacity) { stack_reserve(stack, stack->size); }
}

// very safe yes yes
#define stack_top(stack, type) \
*((type*)stack->elements + stack->top)


#endif