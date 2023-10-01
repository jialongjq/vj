#ifndef _MEDIAPLAYER_INCLUDE
#define _MEDIAPLAYER_INCLUDE
#include <irrKlang.h>
#include <string>
using namespace std;

class MediaPlayer
{
private:
	MediaPlayer();
	MediaPlayer(MediaPlayer const&) = delete;
	void operator=(MediaPlayer const&) = delete;
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundEngine* musicEngine;

	irrklang::ISoundSource* arrowSound;
	irrklang::ISoundSource* openSound;
	irrklang::ISoundSource* exitSound;
	irrklang::ISoundSource* invulnerabilitySound;

	irrklang::ISoundSource* batSound;
	irrklang::ISoundSource* vampireSound;
	irrklang::ISoundSource* transformationSound;
	irrklang::ISoundSource* ghostSound;
	irrklang::ISoundSource* skeletonSound;
	irrklang::ISoundSource* teleportSound;
	irrklang::ISoundSource* clearedSound;
	irrklang::ISoundSource* pointSound;

	irrklang::ISoundSource* frozenSound;
	irrklang::ISoundSource* jumpSound;
	irrklang::ISoundSource* dropSound;
	irrklang::ISoundSource* tileSound;
	irrklang::ISoundSource* tickSound;
	irrklang::ISoundSource* unlockSound;
	irrklang::ISoundSource* shieldSound;
	irrklang::ISoundSource* collectableShownSound;
	irrklang::ISoundSource* collectableCollectedSound;
	
	irrklang::ISoundSource* gameOverSound;
	irrklang::ISoundSource* hallelujahSound;

	irrklang::ISoundSource* shakingSound;
	irrklang::ISoundSource* readySound;
	irrklang::ISoundSource* level1Music;
	irrklang::ISoundSource* level2Music;
	irrklang::ISoundSource* level3Music;
	irrklang::ISoundSource* titleMusic;

	static MediaPlayer *mediaPlayer;

public:
	static MediaPlayer* getInstance();
	void playArrowSound();
	void playOpenSound();
	void playExitSound();
	void playInvulnerabilitySound();

	void playEnemySound(string type);
	void playSpawnSound();
	void playClearedSound();
	void playPointSound();

	void playFrozenSound();
	void playJumpSound();
	void playDropSound();
	void playTileSound();
	void playTickSound();
	void playUnlockSound();
	void playShieldSound();

	void playCollectableShownSound();
	void playCollectableCollectedSound();

	void playGameOverSound();
	void playHallelujahSound();

	void playShakingSound();
	void playReadySound();

	void playLevelMusic(int level);
	void stopLevelMusic(int level);
	void playTitleMusic();

	void stopAll();
};

#endif