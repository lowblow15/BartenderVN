#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

//#define OLC_SOUNDWAVE
//#include "olcSoundWaveEngine.h"

# include <sstream>
#include <iostream>
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "BartenderVN";
	}
	enum NpcsEnum
	{
		drifter = 0,
		officelady = 1,
		trucker = 2,
		marriedwoman = 3,
		changling = 4,
	};
	enum stateEnum
	{
		cutsceneState = 0,
		barState =1,
		EODstate = 3,
		newDayState = 4,
		GameOverState = 5,
		titleState = 6,
	};
	struct Rect
	{
		olc::vf2d pos;
		olc::vf2d size;
	};
	struct ScreenWipe
	{
		Rect box;
		olc::Pixel Color;
		float speed;
		bool rising;
		bool active;
		void setScreenWipe(float x,float y)
		{
			box.pos = olc::vf2d(x, y);
			box.size = { 25,25 };
			Color = olc::WHITE;
			speed = 500;
			rising = true;
			active = true;
		}

	};
	struct mapTiles
	{
		Rect tileRect;
		bool collidable;
		olc::Sprite* tileSpritePtr;
		olc::Decal* tileDecPtr;

	};
	std::vector<std::vector<mapTiles>> Map;
	struct Character
	{
		Rect overworldRect;
		olc::vf2d PortraitPos;
		olc::Sprite* PortraitsprPtr;
		olc::Decal* PortraitDecptr;
		olc::Sprite* overWorldSprPtr;
		olc::Decal* overWorldDecPtr;
	};
	struct npc
	{
		olc::vf2d PortraitPos;
		Rect overWorldRect;
		olc::Sprite* PortraitsprPtr;
		olc::Decal* PortraitDecptr;
		olc::Sprite* overWorldSprPtr;
		olc::Decal* overWorldDecPtr;
		int IDnum;
		std::string favoriteDrink;
		std::string Problem;
		int  CompletionPointsTotal = 0;;
		int MaxCompletionPoints = 100;
		int CompletionPointsInc = 25;
		bool Changling = false;
		bool problemComplete = false;
		void incrementComplete()
		{
			if (problemComplete == false)
			{
				if (CompletionPointsTotal < MaxCompletionPoints)
				{
					CompletionPointsTotal += CompletionPointsInc;
				}
				else
				{
					CompletionPointsTotal = MaxCompletionPoints;
					problemComplete = true;
				}
			}
		}
	};
	Character MainCharacter;
	std::vector <npc>NPCs;
	struct checkBox
	{
		Rect box;
		Rect innerBox;
		bool checked = false;
		std::string buttontitle;
		float stringScale;
		olc::vf2d stringPos;

		void setinnerBox()
		{
			innerBox.pos = olc::vf2d(box.pos.x + 2, box.pos.y + 2);
			innerBox.size = olc::vf2d(box.size.x - 4, box.size.y - 4);
		}
		void setTitle(std::string title, olc::vf2d pos, float scale = 1.25)
		{
			buttontitle = title;
			stringPos = pos;
			stringScale = scale;
		}
	};
	struct windowBox
	{
		Character* MainChar;
		npc* NpcInScene;
		Rect boxOuter;
		Rect boxInner;
		olc::Pixel outerColor;
		olc::Pixel innerColor;
		std::string displayString;
		olc::vf2d StringPos;
		bool selectionMode = false;
		std::vector<std::string> selectionStrings;
		std::vector<olc::vf2d> selectionStringPos;
		std::vector<checkBox> Selections;
		void setBox(olc::vf2d pos, olc::vf2d size, olc::Pixel colorinner, olc::Pixel colorOuter)
		{
			boxInner.pos = pos;
			boxInner.size = size;
			setBorder();

			innerColor = colorinner;
			outerColor = colorOuter;

		}
		void setBorder()
		{
			boxOuter.pos = { boxInner.pos.x - 4, boxInner.pos.y - 4 };
			boxOuter.size = { boxInner.size.x + 8, boxInner.size.y + 8 };
		}
		void setString(std::string inputString)
		{
			displayString = inputString;
		}
		void setActors(Character* thisChar,npc* thisNpc )
		{
			MainChar = thisChar;
			NpcInScene = thisNpc;
		}
		void setStringPos()
		{
			StringPos = { boxInner.pos.x + 5,boxInner.pos.y + 5 };
		}
		void setSelections( int numofSelections)
		{
			int x = 0;
			
			Selections.clear();
			Selections.resize(numofSelections);
			selectionStringPos.clear();
			selectionStringPos.resize(numofSelections);
			for (auto i = Selections.begin(); i < Selections.end(); i++)
			{
				i->box.pos = { boxInner.pos.x + 10,boxInner.pos.y + 10 + 35 * x };
				i->box.size = { 25,25 };
				i->innerBox.pos = { i->box.pos.x+2,i->box.pos.y+2 };
				i->innerBox.size = { i->box.size.x - 4,i->box.size.y - 4 };
				i->buttontitle = selectionStrings[x];
				x++;
			}
			x = 0;
			for (auto i = selectionStringPos.begin(); i < selectionStringPos.end(); i++)
			{
				*i = { Selections[x].box.pos.x + Selections[x].box.size.x + 10,Selections[x].box.pos.y + Selections[x].box.size.y / 2 };
				Selections[x].stringPos = *i;
				x++;
			}
			Selections[0].checked = true;
		}
		void setPortraitPos(float x , float y)
		{
			NpcInScene->PortraitPos = { boxInner.pos.x, boxInner.pos.y - 200 };
			//MainChar->PortraitPos = { boxInner.pos.x + boxInner.size.x - 100,boxInner.pos.y - 200 };
			MainChar->PortraitPos = { x - 100,y - 300};
		}
	};
	struct cutScene
	{
		windowBox SceneWindow;
		std::vector<std::string> npcStrings;
		std::vector<std::string> MainCharStrings;
		int talkState= 0;
		bool Over = false;
		bool lastNPCLine;
		int numOfLines = 0;
		void toggleTalkState()
		{
			if (talkState == 0)
			{
				talkState = 1;
			}
			else
			{
				talkState = 0;
			}
		}
		void NextStringSetter()
		{ 
			if (Over == false)
			{


				toggleTalkState();
				switch (talkState)
				{
				case 0:
					if (numOfLines > 1)
					{

						numOfLines--;
						setNextCharacterString(MainCharStrings[0]);
						MainCharStrings.erase(MainCharStrings.begin());
					}
					else
					{
						ConversationUpdater();
						lastNPCLine = true;
					}
					break;
				case 1:
					if (npcStrings.size() < 1)
					{
						bool breaker = true;
					}
					setNextNpcString(npcStrings[0]);
					npcStrings.erase(npcStrings.begin());
					if (lastNPCLine == true)
					{
						Over = true;
					}
					break;
				default:
					break;
				}
			}
		}
		void setNextNpcString(std::string thisString)
		{
			SceneWindow.setString(thisString);
		}
		void setNextCharacterString(std::string thisString)
		{
			SceneWindow.setString(thisString);
		}
		void SetNumOfLines()
		{
			numOfLines = MainCharStrings.size() + 1;

		}
		void ConversationUpdater()
		{
			SceneWindow.selectionMode = true;
		}

	};
	int Days = 4;
	int GameState = 0;
	int score;
	int nextNpcNum = 0;
	int nextSceneNum = 0;
	bool gameOver = false;
	bool FireOff = false;
	ScreenWipe Wiper;
	bool ScreenWiping = false;
	std::vector<std::vector<cutScene>> Scenes;
	cutScene* CurrentScenePtr;
	npc* ActiveNPC;
	std::vector<std::string> Drinks = {"Manhattan","Martini","Red Wine","Scotch and Soda","Daiquiri","Old Fashioned"};
	std::vector<std::vector<std::string>> Problems;
	std::vector<std::string> sprStrings ={ "./assets/MiddleAgedMan.png","./assets/officelady.png","./assets/trucker.png","./assets/MarriedWoman.png" };
	std::vector<std::string> ChangelingSprStrings = { "./assets/MiddleAgedManChangling.png", "./assets/officeladyChangling.png","./assets/truckerChangling.png", "./assets/MarriedWomanChangling.png" };
	int SelectionNum = 0;
	bool SettingNextScene = false;
	std::vector<int> npcNums = { drifter,officelady,trucker,marriedwoman};
public:
	void parseFile( int npcnum, int scenenum, const std::string& filename, std::vector<std::string>& TheseStrings)
	{
		TheseStrings.clear();
		std::string tokenToCheck = { std::to_string(npcnum) + " " + std::to_string(scenenum) };
		std::ifstream file(filename);
		/*if (!file.is_open()) {
			std::cerr << "Error opening file: " << filename << std::endl;
			return;
		}*/
		if (file.is_open())
		{


			std::string line;
			while (std::getline(file, line)) {
				if (line.find(tokenToCheck) != std::string::npos)
				{
					//std::istringstream iss(line);

					std::string text;

					// Parse NPC number, scene number, and text
				
						// Read the rest of the line as text
						text = line.substr(line.find(" ") + 3);

						TheseStrings.emplace_back(text);

					
				}
			}
		}
		file.close();
	}
	void SelectionNumUpdater(bool increase)
	{
		if (increase == true)
		{
			if (SelectionNum<1)
			{
				SelectionNum++;
			}
			else
			{
				SelectionNum = 0;
			}
		}
		else
		{
			if (SelectionNum>0)
			{
				SelectionNum--;
			}
			else
			{
				SelectionNum = 1;
			}
		}
		clearSelected();
		CurrentScenePtr->SceneWindow.Selections[SelectionNum].checked = true;
	}
	void clearSelected()
	{
		for (auto i = CurrentScenePtr->SceneWindow.Selections.begin(); i < CurrentScenePtr->SceneWindow.Selections.end(); i++)
		{
			i->checked = false;
		}
	}
	
	void DrawCutscene()
	{
		DrawRectDecal(CurrentScenePtr->SceneWindow.boxOuter.pos, CurrentScenePtr->SceneWindow.boxOuter.size);
		FillRectDecal(CurrentScenePtr->SceneWindow.boxInner.pos, CurrentScenePtr->SceneWindow.boxInner.size,olc::BLUE);
		if (CurrentScenePtr->SceneWindow.selectionMode == false)
		{
			DrawStringDecal(CurrentScenePtr->SceneWindow.StringPos, CurrentScenePtr->SceneWindow.displayString);
		}
		else
		{
			for (auto i = CurrentScenePtr->SceneWindow.Selections.begin(); i < CurrentScenePtr->SceneWindow.Selections.end(); i++)
			{
				DrawSelectionBox(*i);
			}
		}
		
		DrawDecal(CurrentScenePtr->SceneWindow.MainChar->PortraitPos, CurrentScenePtr->SceneWindow.MainChar->PortraitDecptr,{.25,.25});
		DrawDecal(CurrentScenePtr->SceneWindow.NpcInScene->PortraitPos, CurrentScenePtr->SceneWindow.NpcInScene->PortraitDecptr, { .25,.25 });
	}
	void DrawSelectionBox(checkBox& thisBox)
	{
		DrawRectDecal(thisBox.box.pos, thisBox.box.size);
		if (thisBox.checked == false)
		{
			FillRectDecal(thisBox.innerBox.pos, thisBox.innerBox.size, olc::Pixel(0, 0, 0, 200));
		}
		else
		{
			FillRectDecal(thisBox.innerBox.pos, thisBox.innerBox.size, olc::Pixel(255,215, 0, 200));
		}
		DrawStringDecal(thisBox.stringPos, thisBox.buttontitle);

	}
	void DayManager()
	{
		if (Days == 0)
		{
			GameStateManager(GameOverState);
			return;
		}
		Days--;
		if (Days == 2)
		{

		}
		GameStateManager(newDayState);
		
		NewDay();
	}
	void NewDay()
	{
		NewDaySceneSetter();
		GameStateManager(cutsceneState);

	}
	void NewDaySceneSetter()
	{
		for (int i = 0; i < Scenes.size(); i++)
		{
			Scenes[i].erase(Scenes[i].begin());
		}
		SetCurrentScene(nextNpcNum, 0);
	}
	void SetCurrentScene(int npcnum , int scenenum)
	{
		CurrentScenePtr = &Scenes[scenenum][npcnum];
		
		CurrentScenePtr->NextStringSetter();
	}
	void PickRandomNPC()
	{
		int randint = rand() % NPCs.size() - 1;
		NPCs[randint].Changling = true;
		NPCs[randint].PortraitsprPtr = new olc::Sprite(ChangelingSprStrings[randint]);
		NPCs[randint].PortraitDecptr = new olc::Decal(NPCs[randint].PortraitsprPtr);
		//ActiveNPC = &NPCs[randint];
		int scenenum = 0;
		for (auto i = Scenes.begin(); i < Scenes.end(); i++)
		{
			for (auto x = i->begin(); x < i->end(); x++)
			{
				if (scenenum >= nextSceneNum)
				{


					if (NPCs[randint].IDnum ==x->SceneWindow.NpcInScene->IDnum )
					{
						x->SceneWindow.NpcInScene=&NPCs[randint];
						parseFile(4, scenenum, "./assets/Story.txt", x->npcStrings);
						parseFile(4, scenenum, "./assets/BartenderReply.txt", x->MainCharStrings);
						parseFile(4, scenenum, "./assets/Choices.txt", x->SceneWindow.selectionStrings);
						x->SceneWindow.setSelections(2);
					}
				}
			}
			scenenum++;
		}

	}

	
	void InitCutscenes(int npcnum,int scenenum)
	{
		Scenes.resize(npcnum);
		for (auto i = Scenes.begin(); i < Scenes.end(); i++)
		{
			for (int x = 0; x < scenenum; x++)
			{
				cutScene newcutscene;
				i->emplace_back(newcutscene);
			}
		}
		int npC = 0;
	    scenenum = 0;
		//going in a straight line not one to the next right now probably quick fix
		for (auto i = Scenes.begin(); i < Scenes.end(); i++)
		{
			/*if (scenenum ==2)
			{
				PickRandomNPC();
			}*/
			for (auto x = i->begin(); x < i->end(); x++)
			{
				
				x->SceneWindow.NpcInScene = &NPCs[npC];
				x->SceneWindow.NpcInScene->IDnum = npC;
				x->SceneWindow.MainChar = &MainCharacter;
				if (x->SceneWindow.NpcInScene->Changling==false)
				{
					parseFile(npC, scenenum, "./assets/Story.txt", x->npcStrings);
					parseFile(npC, scenenum, "./assets/BartenderReply.txt", x->MainCharStrings);
					parseFile(npC, scenenum, "./assets/Choices.txt", x->SceneWindow.selectionStrings);
				}
				else
				{
					parseFile(4, scenenum, "./assets/Story.txt", x->npcStrings);
					parseFile(4, scenenum, "./assets/BartenderReply.txt", x->MainCharStrings);
					parseFile(4, scenenum, "./assets/Choices.txt", x->SceneWindow.selectionStrings);
				}
				x->SetNumOfLines();
				//x->NextStringSetter();

				x->SceneWindow.setBox(olc::vf2d( 5,ScreenHeight() - 150 ), olc::vf2d(  ScreenWidth() - 15, 75 ), olc::BLUE, olc::WHITE);
				x->SceneWindow.setStringPos();
				x->SceneWindow.setPortraitPos(ScreenWidth(),ScreenHeight());
				x->SceneWindow.setSelections(2);
				npC++;
			}
			npC = 0;
			scenenum++;
		}
	}
	void SetNewStringsToDisplay()
	{
		CurrentScenePtr->NextStringSetter();
	}
	void InitNpcs(int NpcNum,std::string sprString)
	{
		npc newNpc;
		newNpc.IDnum = NpcNum;
	
		newNpc.PortraitsprPtr = new olc::Sprite(sprString);
		newNpc.PortraitDecptr = new olc::Decal(newNpc.PortraitsprPtr);
		NPCs.emplace_back(newNpc);
	}
	void SetBartenderSpr()
	{
		MainCharacter.PortraitsprPtr = new olc::Sprite("./assets/Bartender.png");
		MainCharacter.PortraitDecptr = new olc::Decal(MainCharacter.PortraitsprPtr);
	}
	void InitThings()
	{
	
	
		SetBartenderSpr();
		for (int i = 0; i < npcNums.size(); i++)
		{
			InitNpcs(npcNums[i], sprStrings[i]);
		}
	
		InitCutscenes(4, 4);
		
		SetCurrentScene(nextNpcNum, nextSceneNum);
		//CurrentScenePtr->SceneWindow.setSelections( 2);
	}
	void UpdatenextnpcNum()
	{
		if (nextNpcNum<NPCs.size()-1)
		{
			nextNpcNum++;
		}
		else
		{
			nextNpcNum = 0;
			if (nextSceneNum<3)
			{
				nextSceneNum++;
				if (nextSceneNum ==2)
				{
					PickRandomNPC();
				}
			}
			else
			{
				gameOver = true;
			}
			
		}
	}
	void ConfirmSelection()
	{
		if (SelectionNum == 0)
		{
			score += 15;
		}
		CurrentScenePtr->SceneWindow.selectionMode = false;
		CurrentScenePtr->NextStringSetter();

	}
	void GameStateManager(int newState)
	{
		GameState = newState;
	}
	void PlayerControls(float fElapsedTime)
	{
		switch (GameState)
		{
		case cutsceneState:
			if (ScreenWiping == false&&CurrentScenePtr->Over == false)
			{


				if (CurrentScenePtr->SceneWindow.selectionMode == true)
				{


					if (GetKey(olc::Key::RIGHT).bReleased || GetKey(olc::Key::D).bReleased)
					{
						SelectionNumUpdater(true);
					}
					if (GetKey(olc::Key::DOWN).bReleased || GetKey(olc::Key::S).bReleased)
					{
						SelectionNumUpdater(true);
					}
					if (GetKey(olc::Key::LEFT).bReleased || GetKey(olc::Key::A).bReleased)
					{
						SelectionNumUpdater(false);
					}
					if (GetKey(olc::Key::UP).bReleased || GetKey(olc::Key::W).bReleased)
					{
						SelectionNumUpdater(false);
					}

					if (GetKey(olc::Key::ENTER).bReleased)
					{
						ConfirmSelection();
					}
				}
				else
				{


					if (GetKey(olc::Key::ENTER).bReleased)
					{
						CurrentScenePtr->NextStringSetter();
					}
				}
			}
			else
			{
				if (GetKey(olc::Key::ENTER).bReleased)
				{
					FireOff = true;
				}
			}
			break;
		case barState:
			break;
		case EODstate:
			if (GetKey(olc::Key::ENTER).bReleased)
			{
				GameStateManager(newDayState);
			}
			break;
		case newDayState:
			if (GetKey(olc::Key::ENTER).bReleased)
			{
				GameStateManager(cutsceneState);
			}
			break;
		case GameOverState:
			if (GetKey(olc::Key::ENTER).bReleased)
			{
				GameStateManager(titleState);
			}
			break;
		case titleState:
			if (GetKey(olc::Key::ENTER).bReleased)
			{
				GameStateManager(cutsceneState);
			}
			break;
		default:
			break;
		}
	}
	bool OnUserCreate() override
	{
		InitThings();
		// Called once at the start, so create things here
		return true;
	}
	void DrawBar()
	{

	}
	void DrawEOD()
	{

	}
	void DrawNewDay()
	{

	}
	void DrawGameOver()
	{
		DrawStringDecal(olc::vf2d(  25, ScreenHeight() / 2 ), "Thanks for playing!");
		DrawStringDecal(olc::vf2d(  25, (ScreenHeight() / 2) + 50 ), "Your score is " + std::to_string(score));
	}
	void DrawTitleScreen()
	{

	}
	void CutsceneUpdater(float fElapsedTime)
	{
		if (CurrentScenePtr->Over ==true||ScreenWiping == true)
		{
			if (ScreenWiping == false)
			{
				Wiper.setScreenWipe(ScreenWidth() / 2, ScreenHeight() / 2);
				ScreenWiping = true;

			}
			else
			{
				UpdateScreenWipe(fElapsedTime);
				if (Wiper.rising ==false&&SettingNextScene ==false)
				{
					SettingNextScene = true;
					UpdatenextnpcNum();
					SetCurrentScene(nextNpcNum, nextSceneNum);
				}
				if (Wiper.active == false)
				{
					ScreenWiping = false;
					SettingNextScene = false;
				}
			}
		}
	}
	void UpdateScreenWipe(float fElapsedTime)
	{
		if (Wiper.rising ==true)
		{
			if (Wiper.box.size.x <ScreenWidth()+300)
			{
				Wiper.box.size.x += Wiper.speed * fElapsedTime;
				Wiper.box.size.y += Wiper.speed * fElapsedTime;
				Wiper.box.pos.x -= Wiper.speed/2 * fElapsedTime;
				Wiper.box.pos.y -= Wiper.speed/2 * fElapsedTime;
			}
			else
			{
				Wiper.rising = false;
			}
		}
		else
		{
			if (Wiper.box.size.y>0)
			{
				Wiper.box.size.x -= Wiper.speed * fElapsedTime;
				Wiper.box.size.y -= Wiper.speed * fElapsedTime;
				Wiper.box.pos.x += Wiper.speed/2 * fElapsedTime;
				Wiper.box.pos.y += Wiper.speed/2 * fElapsedTime;
			}
			else
			{
				Wiper.active = false;
				FireOff = false;
			}
		}
	}
	void DrawScreenWipe()
	{
		FillRectDecal(Wiper.box.pos, Wiper.box.size, Wiper.Color);
	}
	void DrawScreen(float fElapsedTime)
	{
		switch (GameState)
		{
		case cutsceneState:
			DrawCutscene();
			if (ScreenWiping == true)
			{
				DrawScreenWipe();
			}
			break;
		case barState:
			DrawBar();
			break;
		case EODstate:
			DrawEOD();
			break;
		case newDayState:
			DrawNewDay();

			break;
		case GameOverState:
			DrawGameOver();
			break;
		case titleState:
			DrawTitleScreen();
			break;
		default:
			break;
		}
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		PlayerControls(fElapsedTime);
		DrawScreen(fElapsedTime);
		if (GameState != EODstate)
		{
			if (FireOff == true)
			{


				CutsceneUpdater(fElapsedTime);
			}
			if (gameOver == true)
			{
				GameStateManager(GameOverState);
			}
			
		}
		else
		{
			DayManager();
		}
		return true;
	}
};
//switch (GameState)
//{
//case cutsceneState:
//	DrawCutscene();
//	break;
//case barState:
//	break;
//case EODstate:
//	break;
//case newDayState:
//	break;
//default:
//	break;
//}

int main()
{
	srand(std::time(NULL));
	Example demo;
	if (demo.Construct(600, 400, 4, 4))
		demo.Start();

	return 0;
}
