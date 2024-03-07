#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
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
		void setSelections(std::vector<std::string> theseStrings, int numofSelections)
		{
			int x = 0;
			selectionStrings.clear();
			selectionStrings = theseStrings;
			Selections.clear();
			Selections.resize(numofSelections);
			selectionStringPos.clear();
			selectionStringPos.resize(numofSelections);
			for (auto i = Selections.begin(); i < Selections.end(); i++)
			{
				i->box.pos = { boxInner.pos.x + 10,boxInner.pos.y + 10 + 10 * x };
				x++;
			}
			x = 0;
			for (auto i = selectionStringPos.begin(); i < selectionStringPos.end(); i++)
			{
				*i = { Selections[x].box.pos.x + Selections[x].box.size.x + 10,Selections[x].box.pos.y + Selections[x].box.size.y / 2 };
				x++;
			}
		}
	};
	struct cutScene
	{
		windowBox SceneWindow;
		std::vector<std::string> npcStrings;
		std::vector<std::string> MainCharStrings;
		int talkState= 0;
		bool Over = false;
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
			toggleTalkState();
			switch (talkState)
			{
			case 0:
				if (numOfLines >= 0)
				{

					numOfLines--;
					setNextCharacterString(MainCharStrings[0]);
					MainCharStrings.erase(MainCharStrings.begin());
				}
				else
				{
					ConversationUpdater();
				}
				break;
			case 1:
				setNextNpcString(npcStrings[0]);
				npcStrings.erase(npcStrings.begin());
				break;
			default:
				break;
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
	std::vector<std::vector<cutScene>> Scenes;
	cutScene* CurrentScenePtr;
	npc* ActiveNPC;
	std::vector<std::string> Drinks = {"Manhattan","Martini","Red Wine","Scotch and Soda","Daiquiri","Old Fashioned"};
	std::vector<std::vector<std::string>> Problems;
	std::vector<std::string> sprStrings;


	std::vector<int> npcNums = { drifter,officelady,trucker,marriedwoman};
public:
	void parseFile( int npcnum, int scenenum, const std::string& filename, std::vector<std::string>& TheseStrings)
	{
		TheseStrings.clear();

		std::ifstream file(filename);
		/*if (!file.is_open()) {
			std::cerr << "Error opening file: " << filename << std::endl;
			return;
		}*/
		if (file.is_open())
		{


			std::string line;
			while (std::getline(file, line)) {
				std::istringstream iss(line);

				std::string text;

				// Parse NPC number, scene number, and text
				if (iss >> npcnum >> scenenum) {
					// Read the rest of the line as text
					text = line.substr(line.find(" ") + 1);

					TheseStrings.emplace_back(text);

				}
			}
		}
		file.close();
	}
	void DrawCutscene()
	{
		DrawRectDecal(CurrentScenePtr->SceneWindow.boxOuter.pos, CurrentScenePtr->SceneWindow.boxOuter.size);
		FillRectDecal(CurrentScenePtr->SceneWindow.boxInner.pos, CurrentScenePtr->SceneWindow.boxInner.size,olc::BLUE);
		DrawStringDecal(CurrentScenePtr->SceneWindow.StringPos, CurrentScenePtr->SceneWindow.displayString);
		/*DrawDecal(CurrentScenePtr->SceneWindow.MainChar->PortraitPos, CurrentScenePtr->SceneWindow.MainChar->PortraitDecptr);
		DrawDecal(CurrentScenePtr->SceneWindow.NpcInScene->PortraitPos, CurrentScenePtr->SceneWindow.NpcInScene->PortraitDecptr);*/
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
	}
	void SetCurrentScene(int npcnum , int scenenum)
	{
		CurrentScenePtr = &Scenes[npcnum][scenenum];
	}
	void PickRandomNPC()
	{
		int randint = rand() % NPCs.size() - 1;
		ActiveNPC = &NPCs[randint];

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
		for (auto i = Scenes.begin(); i < Scenes.end(); i++)
		{
			for (auto x = i->begin(); x < i->end(); x++)
			{
				
				x->SceneWindow.NpcInScene = &NPCs[npC];
				x->SceneWindow.MainChar = &MainCharacter;
				/*parseFile(npC, scenenum, "./BartenderVN/assets/Story.txt", x->npcStrings);
				parseFile(npC, scenenum, "./BartenderVN/assets/BartenderReply.txt", x->MainCharStrings);
				parseFile(npC, scenenum, "./BartenderVN/assets/Choices.txt", x->SceneWindow.selectionStrings);*/
				parseFile(npC, scenenum, "./assets/Story.txt", x->npcStrings);
				parseFile(npC, scenenum, "./assets/BartenderReply.txt", x->MainCharStrings);
				parseFile(npC, scenenum, "./assets/Choices.txt", x->SceneWindow.selectionStrings);

				x->NextStringSetter();

				x->SceneWindow.setBox(olc::vf2d( 5,ScreenHeight() - 150 ), olc::vf2d(  ScreenWidth() - 15, 125 ), olc::BLUE, olc::WHITE);
				
			}
			npC++;
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
	
	void InitThings()
	{
	
	

		for (int i = 0; i < npcNums.size(); i++)
		{
			InitNpcs(npcNums[i], "");
		}
		for (int i = 0; i < npcNums.size(); i++)
		{
			InitCutscenes(npcNums[i], 7);
		}
		SetCurrentScene(nextNpcNum, nextSceneNum);
	}
	void UpdatenextnpcNum()
	{
		if (nextNpcNum<NPCs.size())
		{
			nextNpcNum++;
		}
		else
		{
			nextNpcNum = 0;
			UpdateNextSceneNum();
		}
	}
	void UpdateNextSceneNum()
	{
		if (nextSceneNum < Scenes[0].size())
		{
			nextSceneNum++;
			GameStateManager(EODstate);
		}
		else
		{
			gameOver = true;
		}
	}
	void GameStateManager(int newState)
	{
		GameState = newState;
	}
	void PlayerControls()
	{
		switch (GameState)
		{
		case cutsceneState:
			if (GetKey(olc::Key::RIGHT).bReleased||GetKey(olc::Key::D).bReleased)
			{

			}
			if (GetKey(olc::Key::DOWN).bReleased || GetKey(olc::Key::S).bReleased)
			{

			}
			if (GetKey(olc::Key::LEFT).bReleased || GetKey(olc::Key::A).bReleased)
			{

			}
			if (GetKey(olc::Key::UP).bReleased || GetKey(olc::Key::W).bReleased)
			{

			}
			if (GetKey(olc::Key::ENTER).bReleased)
			{

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

	}
	void DrawTitleScreen()
	{

	}
	void DrawScreen(float fElapsedTime)
	{
		switch (GameState)
		{
		case cutsceneState:
			DrawCutscene();
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
		PlayerControls();
		DrawScreen(fElapsedTime);
		if (GameState != EODstate)
		{

			
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
	srand(time(0));
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
