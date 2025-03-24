#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 1000
#define HEIGHT 1000
#define BG 0X00000000
#define mass1_color 0xffffffff

struct world_consts
{
	double gravity;
};

struct Mass
{
	double x;
	double y;
	double r;
	double mass;
};
struct Vel
{
	double x;
	double y;
};


void DrawMass(SDL_Surface* surface, struct Mass mass, double color)
{
	double r_sq = pow(mass.r,2);
    for (double x = mass.x-mass.r; x<=mass.x+mass.r; x++)
    {
        for(double y = mass.y-mass.r; y<=mass.y+mass.r; y++)
        {
            double distance_sq =pow(x-mass.x,2) + pow(y-mass.y,2);
            if(distance_sq < r_sq)
            {
                SDL_Rect pixel =(SDL_Rect){x,y,1,1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }

}
	
int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("3-mass-problem",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface =SDL_GetWindowSurface(window);
    double sim_run = 1;
    SDL_Event event;
    SDL_Rect rect = (SDL_Rect) {0,0,WIDTH,HEIGHT};
    

    struct Mass m1 = {WIDTH/2,HEIGHT/2-60 - 60,20,40000000000000};
    struct Mass m2 = {WIDTH/2 ,HEIGHT/2 ,20,50000000000000};
    struct Mass m3 = {WIDTH/2 - 80,HEIGHT/2,20,30000000000000};
    struct Vel vel1 = {10,0};
	struct Vel vel2 = {0,0};
	struct Vel vel3 = {0,0};
	double distance1_3 = 0;
	double distance2_3 = 0;
	double distance1_2 = 0;
	double Force1_2 = 0;
	double Force2_3 = 0;
	double Force1_3 = 0;
	double G = 6*pow(10,-11);

    while(sim_run){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				sim_run = 0;
			}

			
		}
		SDL_FillRect(surface, &rect, BG);
		DrawMass(surface, m1, mass1_color);
		DrawMass(surface, m2, mass1_color);
		DrawMass(surface, m3, mass1_color);

		distance1_2 = sqrt(pow(m1.x-m2.x,2) + pow(m1.y - m2.y,2));
		distance1_3 = sqrt(pow(m1.x-m3.x,2) + pow(m1.y - m3.y,2));
		distance2_3 = sqrt(pow(m2.x-m3.x,2) + pow(m2.y - m3.y,2));

		Force1_2 = G * m1.mass * m2.mass/pow(distance1_2,2);
		Force2_3 = G * m2.mass * m3.mass/pow(distance2_3,2);
		Force1_3 = G * m1.mass * m3.mass/pow(distance1_3,2);
			
		vel1.x -= Force1_2 * ((m1.x - m2.x)/distance1_2); + Force1_3 * ((m1.x-m3.x)/distance1_3);
		vel1.y -= Force1_2 * ((m1.y - m2.y)/distance1_2); + Force1_3 * ((m1.y-m3.y)/distance1_3);
		vel2.x -= Force1_2 * ((m2.x - m1.x)/distance1_2); + Force2_3 * ((m2.x-m3.x)/distance2_3);
		vel2.y -= Force1_2 * ((m2.y - m1.y)/distance1_2); + Force2_3 * ((m2.y-m3.y)/distance2_3);
		vel3.x -= Force2_3 * ((m3.x - m2.x)/distance2_3); + Force1_3 * ((m3.x-m1.x)/distance1_3);
		vel3.y -= Force2_3 * ((m3.y - m2.y)/distance2_3); + Force1_3 * ((m3.y-m1.y)/distance1_3);

		m1.x += 4 + vel1.x/m1.mass;
		m1.y += vel1.y/m1.mass;
		m2.x += vel2.x/m2.mass;
		m2.y += 5 + vel2.y/m2.mass;
		m3.x += vel3.x/m3.mass;
		m3.y += -3 + vel3.y/m3.mass;


		printf("distance %f\n",distance1_2 );
		printf("vel %f\n",vel1.x );

		SDL_UpdateWindowSurface(window);
    	SDL_Delay(10); 	
	}
	  
	
}