#include "Drawable.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include <memory>

class Collision
{
public:
	static void CheckForEnemyCollision(std::vector<std::unique_ptr<Enemy>>& enemies, Enemy& enemy, Player& player)
	{
		if (player.GetPlayerBullet()->GetAABB().top < enemy.GetAABB().bottom && player.GetPlayerBullet()->GetAABB().bottom > enemy.GetAABB().top
			&& player.GetPlayerBullet()->GetAABB().left < enemy.GetAABB().right && player.GetPlayerBullet()->GetAABB().right > enemy.GetAABB().left && player.GetPlayerBullet()->BulletFired() && enemy.IsAlive())
		{
			enemy.SetDead();
			player.GetPlayerBullet()->SetNotFired();
			player.IncreaseScore();
			for (auto i = enemies.begin(), end = enemies.end(); i != end; i++)
			{
				(*i)->IncreaseSpeed();
			}
		}
	}
	static void CheckForEnemyHitBottom(Enemy& enemy, Player& player)
	{
		if (enemy.GetAABB().bottom >= 595 && enemy.IsAlive())
		{
			while (player.GetLives() > 0)
			{
				player.PlayerHit();
				player.LoseLife();
				player.PlayerNotHit();
			}
		}
	}
	static void CheckForPlayerCollision(Enemy& enemy, Player& player)
	{
		if (enemy.GetEnemyBullet()->GetAABB().top < player.GetAABB().bottom && enemy.GetEnemyBullet()->GetAABB().bottom > player.GetAABB().top
			&& enemy.GetEnemyBullet()->GetAABB().left < player.GetAABB().right && enemy.GetEnemyBullet()->GetAABB().right > player.GetAABB().left && enemy.GetEnemyBullet()->BulletFired())
		{
			player.PlayerHit();
			enemy.GetEnemyBullet()->SetNotFired();
		}
		if (player.IsPlayerHit())
		{
			player.ResetPlayer();
		}
	}
	static void CheckForPlayerEnemyCollisionCollision(Enemy& enemy, Player& player)
	{
		if (enemy.GetAABB().top < player.GetAABB().bottom && enemy.GetAABB().bottom > player.GetAABB().top
			&& enemy.GetAABB().left < player.GetAABB().right && enemy.GetAABB().right > player.GetAABB().left && !player.IsPlayerHit() && enemy.IsAlive())
		{
			enemy.SetDead();
			player.PlayerHit();
			player.IncreaseScore();
		}
		if (player.IsPlayerHit())
		{
			player.ResetPlayer();
		}
	}
};