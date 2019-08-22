#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <cstdint>
#include <memory>

#include <QMap>
#include <QString>
#include <QStringBuilder>
#include <QMutex>


class FSArchiveHandler;
class FSArchiveFile;
class QProgressDialog;

namespace Game
{

enum VersionMasks
{
	USER_MASK = 0xFFFFF,
	USER_MASK_BS = 0xFFFF,
	BS_MASK = 0xFF
};

constexpr uint64_t VersionDef( uint64_t major = 0, uint64_t minor = 0, uint64_t patch = 0, uint64_t inter = 0,
							uint64_t user = 0, uint64_t bs_min = 0, uint64_t bs_max = 0 )
{
	return (bs_min > 0)
		? (major << 24) | (minor << 16) | (patch << 8) | inter | ((user & USER_MASK_BS) << 32)
				| ((bs_min & BS_MASK) << 48) | ((((bs_max < bs_min) ? bs_min : bs_max) & BS_MASK) << 56)
		: (major << 24) | (minor << 16) | (patch << 8) | inter | ((user & USER_MASK) << 32);
}

constexpr uint32_t VersionInt( uint64_t version )
{
	return (uint32_t)version;
}

enum GameMode
{
	OTHER,
	MORROWIND,
	OBLIVION,
	FALLOUT_3,
	FALLOUT_NV,
	SKYRIM,
	SKYRIM_SE,
	FALLOUT_4,
	FALLOUT_76,

	NUM_GAMES,

	// Not a physical game, exclude from NUM_GAMES count
	FALLOUT_3NV,
};

using GameMap = QMap<GameMode, QString>;
using GameEnabledMap = QMap<GameMode, bool>;
using ResourceListMap = QMap<GameMode, QStringList>;

using namespace std::string_literals;

static auto beth = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Bethesda Softworks\\%1");
static auto msft = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%1");


static GameMap STRING = {
	{MORROWIND, "Morrowind"},
	{OBLIVION, "Oblivion"},
	{FALLOUT_3, "Fallout 3"},
	{FALLOUT_NV, "Fallout: New Vegas"},
	{SKYRIM, "Skyrim"},
	{SKYRIM_SE, "Skyrim SE"},
	{FALLOUT_4, "Fallout 4"},
	{FALLOUT_76, "Fallout 76"},
	{OTHER, "Other Games"}
};

static GameMap KEY = {
	{MORROWIND, beth.arg("Morrowind")},
	{OBLIVION, beth.arg("Oblivion")},
	{FALLOUT_3, beth.arg("Fallout3")},
	{FALLOUT_NV, beth.arg("FalloutNV")},
	{SKYRIM, beth.arg("Skyrim")},
	{SKYRIM_SE, beth.arg("Skyrim Special Edition")},
	{FALLOUT_4, beth.arg("Fallout4")},
	{FALLOUT_76, msft.arg("Fallout 76")},
	{OTHER, ""}
};

static GameMap DATA = {
	{MORROWIND, "Data Files"},
	{OBLIVION, "Data"},
	{FALLOUT_3, "Data"},
	{FALLOUT_NV, "Data"},
	{SKYRIM, "Data"},
	{SKYRIM_SE, "Data"},
	{FALLOUT_4, "Data"},
	{FALLOUT_76, "Data"},
	{OTHER, ""}
};

static ResourceListMap FOLDERS = {
	{MORROWIND, {"."}},
	{OBLIVION, {"."}},
	{FALLOUT_3, {"."}},
	{FALLOUT_NV, {"."}},
	{SKYRIM, {"."}},
	{SKYRIM_SE, {"."}},
	{FALLOUT_4, {".", "Textures"}},
	{FALLOUT_76, {".", "Textures"}},
	{OTHER, {}}
};

enum GameVersion : uint64_t
{
	V3_3_0_13 = VersionDef(3, 3, 0, 13),
	V4_0_0_0 = VersionDef(4),
	V4_0_0_2 = VersionDef(4, 0, 0, 2),
	V4_1_0_12 = VersionDef(4, 1, 0, 12),
	V4_2_0_2 = VersionDef(4, 2, 0, 2),
	V4_2_1_0 = VersionDef(4, 2, 1, 0),
	V4_2_2_0 = VersionDef(4, 2, 2, 0),
	V10_0_1_0 = VersionDef(10, 0, 1, 0),
	V10_0_1_2 = VersionDef(10, 0, 1, 2, 0, 1, 3),
	V10_1_0_0 = VersionDef(10, 1, 0, 0),
	V10_1_0_101 = VersionDef(10, 1, 0, 101, 10, 4),
	V10_1_0_106 = VersionDef(10, 1, 0, 106, 10, 5),
	V10_2_0_0 = VersionDef(10, 2, 0, 0),
	V10_2_0_0__1 = VersionDef(10, 2, 0, 0, 1),
	V10_2_0_0__10 = VersionDef(10, 2, 0, 0, 10, 6, 9),
	V10_2_0_1 = VersionDef(10, 2, 0, 1),
	V10_3_0_1 = VersionDef(10, 3, 0, 1),
	V10_4_0_1 = VersionDef(10, 4, 0, 1),
	V20_0_0_4 = VersionDef(20, 0, 0, 4),
	V20_0_0_4__10 = VersionDef(20, 0, 0, 4, 10, 11),
	V20_0_0_4__11 = VersionDef(20, 0, 0, 4, 11, 11),
	V20_0_0_5_OBL = VersionDef(20, 0, 0, 5, 10, 11),
	V20_1_0_3 = VersionDef(20, 1, 0, 3),
	V20_2_0_7 = VersionDef(20, 2, 0, 7),
	V20_2_0_7__11_1 = VersionDef(20, 2, 0, 7, 11, 14),
	V20_2_0_7__11_2 = VersionDef(20, 2, 0, 7, 11, 16),
	V20_2_0_7__11_3 = VersionDef(20, 2, 0, 7, 11, 21),
	V20_2_0_7__11_4 = VersionDef(20, 2, 0, 7, 11, 24),
	V20_2_0_7__11_5 = VersionDef(20, 2, 0, 7, 11, 25),
	V20_2_0_7__11_6 = VersionDef(20, 2, 0, 7, 11, 26),
	V20_2_0_7__11_7 = VersionDef(20, 2, 0, 7, 11, 27, 28),
	V20_2_0_7__11_8 = VersionDef(20, 2, 0, 7, 11, 30, 33),
	V20_2_0_7_FO3 = VersionDef(20, 2, 0, 7, 11, 34),
	V20_2_0_7_SKY = VersionDef(20, 2, 0, 7, 11, 83),
	V20_2_0_7_SSE = VersionDef(20, 2, 0, 7, 11, 100),
	V20_2_0_7_FO4 = VersionDef(20, 2, 0, 7, 11, 130),
	V20_2_0_7_F76 = VersionDef(20, 2, 0, 7, 11, 155),
	V20_2_0_8 = VersionDef(20, 2, 0, 8),
	V20_3_0_1 = VersionDef(20, 3, 0, 1),
	V20_3_0_2 = VersionDef(20, 3, 0, 2),
	V20_3_0_3 = VersionDef(20, 3, 0, 3),
	V20_3_0_6 = VersionDef(20, 3, 0, 6),
	V20_3_0_9 = VersionDef(20, 3, 0, 9),
	V20_3_0_9_DIV2 = VersionDef(20, 3, 0, 9, 0x20000), // TODO: 0x30000?
	V20_5_0_0 = VersionDef(20, 5, 0, 0),
	V20_6_0_0 = VersionDef(20, 6, 0, 0),
	V20_6_5_0_DEM = VersionDef(20, 6, 5, 0, 17),
	V30_0_0_2 = VersionDef(30, 0, 0, 2),
	V30_1_0_1 = VersionDef(30, 1, 0, 1),
	V30_1_0_3 = VersionDef(30, 1, 0, 3),
	V30_2_0_3 = VersionDef(30, 2, 0, 3),
};

enum BSVersion
{
	BSSTREAM_1 = 1,
	BSSTREAM_3 = 3,
	BSSTREAM_4 = 4,
	BSSTREAM_5 = 5,
	BSSTREAM_6 = 6,
	BSSTREAM_7 = 7,
	BSSTREAM_8 = 8,
	BSSTREAM_9 = 9,
	BSSTREAM_11 = 11,
	BSSTREAM_14 = 14,
	BSSTREAM_16 = 16,
	BSSTREAM_21 = 21,
	BSSTREAM_24 = 24,
	BSSTREAM_25 = 25,
	BSSTREAM_26 = 26,
	BSSTREAM_27 = 27,
	BSSTREAM_28 = 28,
	BSSTREAM_30 = 30,
	BSSTREAM_31 = 31,
	BSSTREAM_32 = 32,
	BSSTREAM_33 = 33,
	BSSTREAM_34 = 34,
	BSSTREAM_83 = 83,
	BSSTREAM_100 = 100,
	BSSTREAM_130 = 130,
	BSSTREAM_155 = 155,
};

QString StringForMode(GameMode game);
GameMode ModeForString(QString game);

class GameManager
{
public:
	GameManager();

	static GameMode GetGame(uint32_t version, uint32_t user, uint32_t bsver);

	static GameManager * get();
	static void del();

	static QList <FSArchiveFile *> get_archive_handles(Game::GameMode game);
	static bool archive_contains_folder(const QString& archive, const QString& folder);

	static QString get_game_path(const GameMode game);
	static QString get_game_path(const QString& game);
	static QString get_data_path(const GameMode game);
	static QString get_data_path(const QString& game);
	static QStringList get_folder_list(const GameMode game);
	static QStringList get_folder_list(const QString& game);
	static QStringList get_archive_list(const GameMode game);
	static QStringList get_archive_list(const QString& game);

	static QStringList get_existing_folders_list(const GameMode game);
	static QStringList get_existing_folders_list(const QString& game);
	static QStringList get_archive_file_list(const GameMode game);
	static QStringList get_archive_file_list(const QString& game);
	static QStringList get_filtered_archives_list(const QStringList& list, const QString& folder);

	static void set_game_status(const GameMode game, bool status);
	static void set_game_status(const QString& game, bool status);
	static bool get_game_status(const GameMode game);
	static bool get_game_status(const QString& game);

	static void update_game(const QString& game, const QString& path);
	static void update_folders(const QString& game, const QStringList& list);
	static void update_archives(const QString& game, const QStringList& list);

	void initialize(int& manager_version, QProgressDialog* dlg = nullptr);
	void update(int& manager_version, QProgressDialog* dlg = nullptr);

	void save();
	void load();
	void reset_archive_handles();

	struct GameInfo
	{
		GameMode id = OTHER;
		QString name;
		QString path;
		bool status = false;
	};

private:
	QMutex mutex;

	GameMap game_paths;
	GameEnabledMap game_status;
	ResourceListMap game_folders;
	ResourceListMap game_archives;

	QMap<Game::GameMode, QList<std::shared_ptr<FSArchiveHandler>>> handles;
};

} // end namespace Game

#endif // GAMEMANAGER_H
