#include "GTAFilesChecker.h"
#include <QDir>

const QHash<QString, QString> GTAFilesChecker::s_hashes = {
	{"bink2w64.dll", "3722f03c488093cb2631b5412d4f12d9"},
	{"commandline.txt", "na"},
	{"common.rpf", "bbe724acef5f9fcfd85a38c4e35e2f00"},
	{"d3dcompiler_46.dll", "7ea872c2f9803cfb4223098b85e70cc0"},
	{"d3dcsx_46.dll", "8355e491fa90ca00045be22bb556b213"},
	{"GFSDK_ShadowLib.win64.dll", "f2c348c5aaff0c420f4dce3abc1bbad6"},
	{"GFSDK_TXAA.win64.dll", "167385d13443035ff68643b2c0c59a4d"},
	{"GFSDK_TXAA_AlphaResolve.win64.dll", "ea04393624856f44854cace25b50ce3c"},
	{"GPUPerfAPIDX11-x64.dll", "121044fe4ae47114dfccd15e399df399"},
	{"GTA5.exe", "na"},
	{"GTAVLanguageSelect.exe", "na"},
	{"GTAVLauncher.exe", "na"},
	{"NvPmApi.Core.win64.dll", "2041025c15e6ff893dcbb5ed63fdb2f8"},
	{"PlayGTAV.exe", "na"},
	{"version.txt", "na"},
	{"x64a.rpf", "683610e269ba60c5fcc7a9f6d1a8bfd5"},
	{"x64b.rpf", "70af24cd4fe2c8ee58edb902f018a558"},
	{"x64c.rpf", "2a0f6f1c35ad567fe8e56b9c9cc4e4c6"},
	{"x64d.rpf", "c8757b052ab5079c7749bcce02538b2e"},
	{"x64e.rpf", "e5416c0b0000dad4014e0c5e9b878ff9"},
	{"x64f.rpf", "5c6fc965d56ae6d422cd6cbe5a65a3a5"},
	{"x64g.rpf", "1d8a64b337c3e07dffec0f53530cdb8e"},
	{"x64h.rpf", "fe657d9282df303b080c3a2f6771c9ea"},
	{"x64i.rpf", "bb271d313467465d62c75e208236487b"},
	{"x64j.rpf", "143deee4c7699b9f07ef21d43ae0915b"},
	{"x64k.rpf", "da2c88b4ca69c99a86868a9433084a9d"},
	{"x64l.rpf", "f4307b005a3e90192f235959621781d1"},
	{"x64m.rpf", "a1304d84875747aa7405465d37d3c6fb"},
	{"x64n.rpf", "c48a14fe1c301360a16e8b0c5472fd1d"},
	{"x64o.rpf", "6715a4eabbbc8868f15630bf917db49a"},
	{"x64p.rpf", "6ad56befada1db7cccd9cea7834c825b"},
	{"x64q.rpf", "ff6d09527d7fdc005d3fa78435e09c8a"},
	{"x64r.rpf", "1465c9da5cc17b68f14915b6c1d815bc"},
	{"x64s.rpf", "2c6e61201eb4f60d5c3c1e9ae6d67a32"},
	{"x64t.rpf", "4c15a54a4c9573d7a0bcfa4689d9d1ed"},
	{"x64u.rpf", "2c9cff0cc5f99ad2218e4c4de39881b7"},
	{"x64v.rpf", "db647120263d0282b6f6c555f6112a1c"},
	{"x64w.rpf", "46a4abe50bfc78c30c0173d888cf2c4a"},
	{"x64/audio/audio_rel.rpf", "3f674ab6f7ce814cd4fcf21ad5536165"},
	{"x64/audio/occlusion.rpf", "5be62ba4ad701711c5754256dd38c184"},
	{"x64/audio/sfx/ANIMALS.rpf", "dee58a003406da3553f78e534622ac50"},
	{"x64/audio/sfx/ANIMALS_FAR.rpf", "2a7a5ee23da6459b2ebd2c04dbe1b846"},
	{"x64/audio/sfx/ANIMALS_NEAR.rpf", "647cea419643808e7ffc82b1d311fa13"},
	{"x64/audio/sfx/CUTSCENE_MASTERED_ONLY.rpf", "995f71489084297f31943373c619f68e"},
	{"x64/audio/sfx/DLC_GTAO.rpf", "c2696f76149ac57fa1a61636762aa4c7"},
	{"x64/audio/sfx/INTERACTIVE_MUSIC.rpf", "c07eb284089b4b9db7fcebe7029cbd54"},
	{"x64/audio/sfx/ONESHOT_AMBIENCE.rpf", "4ed22957a50553182cedf5510d1e8e36"},
	{"x64/audio/sfx/PAIN.rpf", "08e42851ff2ee25ef844101bb4eb125e"},
	{"x64/audio/sfx/POLICE_SCANNER.rpf", "30e1ddea02d92e334523301b37bbc2cf"},
	{"x64/audio/sfx/PROLOGUE.rpf", "6ed5fd973aee29d1043280a95fd1374a"},
	{"x64/audio/sfx/RADIO_01_CLASS_ROCK.rpf", "88f6352fd3f00f61534749268c1ec772"},
	{"x64/audio/sfx/RADIO_02_POP.rpf", "9c4ffb44058b17364282e49719c6615f"},
	{"x64/audio/sfx/RADIO_03_HIPHOP_NEW.rpf", "a64dddc3087fdf48d0614dd36212e1a2"},
	{"x64/audio/sfx/RADIO_04_PUNK.rpf", "7817e46dc9a0fa29803e233451041ecb"},
	{"x64/audio/sfx/RADIO_05_TALK_01.rpf", "30684ca056f66e89377d68c9f2a18dad"},
	{"x64/audio/sfx/RADIO_06_COUNTRY.rpf", "f7fdd6057f57a608a4ffac3388296ed7"},
	{"x64/audio/sfx/RADIO_07_DANCE_01.rpf", "3d88e531a07149775eb77108c9bd64e6"},
	{"x64/audio/sfx/RADIO_08_MEXICAN.rpf", "6fde3f68a4069a96f6a4bcae32efbc36"},
	{"x64/audio/sfx/RADIO_09_HIPHOP_OLD.rpf", "b3ce929628e98d781b99e65d55aa796f"},
	{"x64/audio/sfx/RADIO_11_TALK_02.rpf", "6d2491f98bf0721a49aa20fcec24a979"},
	{"x64/audio/sfx/RADIO_12_REGGAE.rpf", "55fa363694db588f0d7fc2d1c04a4bd4"},
	{"x64/audio/sfx/RADIO_13_JAZZ.rpf", "2f864478a763fae03a195df99accb625"},
	{"x64/audio/sfx/RADIO_14_DANCE_02.rpf", "9ce6c2c68e98c7040ab6c1c7ceca7b43"},
	{"x64/audio/sfx/RADIO_15_MOTOWN.rpf", "8ebeb03478b905ff57dfaeb5bafd022c"},
	{"x64/audio/sfx/RADIO_16_SILVERLAKE.rpf", "ea96f48138bd53d5b92cd0999b4b9592"},
	{"x64/audio/sfx/RADIO_17_FUNK.rpf", "b29cb8ead7e02ac7f2df77472ee8e0be"},
	{"x64/audio/sfx/RADIO_18_90S_ROCK.rpf", "31477aa6eacea5bc981e492c2bef29f1"},
	{"x64/audio/sfx/RADIO_ADVERTS.rpf", "e12223a4fd61efc90e75b5d4d8bc6f76"},
	{"x64/audio/sfx/RADIO_NEWS.rpf", "058c51b09fd4be0bfb0d7adfb4d7f965"},
	{"x64/audio/sfx/RESIDENT.rpf", "df2407e6dec6b5fd18034ef0f47ad6bb"},
	{"x64/audio/sfx/SCRIPT.rpf", "b3577479e9bf194c58df5e404c0ee00f"},
	{"x64/audio/sfx/SS_AC.rpf", "829ba72532ee84a204f44c552dcc9da9"},
	{"x64/audio/sfx/SS_DE.rpf", "161caaa12df6b6729aa0b03de5da00f0"},
	{"x64/audio/sfx/SS_FF.rpf", "4330b09d4d707484987afb1279d1bda1"},
	{"x64/audio/sfx/SS_GM.rpf", "1f3a143f4d4d7101aaeab776b6fc7edd"},
	{"x64/audio/sfx/SS_NP.rpf", "f32c9414327fbebfbf41d6663241d3c6"},
	{"x64/audio/sfx/SS_QR.rpf", "71d13bfa850457d7bdc9aeee9f65e1cf"},
	{"x64/audio/sfx/SS_ST.rpf", "5f17f2ae91ea8ba3e35a937e515d1e19"},
	{"x64/audio/sfx/SS_UZ.rpf", "a325ab1785d255c24592aed6a9c5b52e"},
	{"x64/audio/sfx/STREAMED_AMBIENCE.rpf", "8af66d9ad9de11227e95192ec65448b6"},
	{"x64/audio/sfx/STREAMED_VEHICLES.rpf", "84b960d31585d589af1701bdc48c3921"},
	{"x64/audio/sfx/STREAMED_VEHICLES_GRANULAR.rpf", "a34dbb719d6098d8cfdf5f51b0bb9b82"},
	{"x64/audio/sfx/STREAMED_VEHICLES_GRANULAR_NPC.rpf", "511f2ab0c22a55caf096ada395bebe55"},
	{"x64/audio/sfx/STREAMED_VEHICLES_LOW_LATENCY.rpf", "15076fe0ba7f57112882a179c4db0a65"},
	{"x64/audio/sfx/STREAMS.rpf", "9b1fc10f15e2115f0945ee848120aa46"},
	{"x64/audio/sfx/S_FULL_AMB_F.rpf", "8b4398938db36a617877ba3c1db47247"},
	{"x64/audio/sfx/S_FULL_AMB_M.rpf", "8e2028e2e0f1d407cb27e354b6c3e12d"},
	{"x64/audio/sfx/S_FULL_GAN.rpf", "193804dce2772060149c8e897d2f89d8"},
	{"x64/audio/sfx/S_FULL_SER.rpf", "69e05ebbe2c4839534d8a8e850610dc6"},
	{"x64/audio/sfx/S_MINI_AMB.rpf", "0365ec49c281fc58d6719604bb0719b2"},
	{"x64/audio/sfx/S_MINI_GAN.rpf", "e87de77e82fd106c29a2fd42d915961f"},
	{"x64/audio/sfx/S_MINI_SER.rpf", "6917a980d346317cbfcc8f2d474d7111"},
	{"x64/audio/sfx/S_MISC.rpf", "54503436d735aad5d4e9327e6bf89e99"},
	{"x64/audio/sfx/WEAPONS_PLAYER.rpf", "b4a3c9bfd7a818dc47e34ec05fc946da"},
	{"x64/data/errorcodes/american.txt", "a27add49d85dfe94ca71c718e7387e25"},
	{"x64/data/errorcodes/chinese.txt", "a27add49d85dfe94ca71c718e7387e25"},
	{"x64/data/errorcodes/french.txt", "59f970b96d482fe0b34bff8a4edeb61d"},
	{"x64/data/errorcodes/german.txt", "59f970b96d482fe0b34bff8a4edeb61d"},
	{"x64/data/errorcodes/italian.txt", "59f970b96d482fe0b34bff8a4edeb61d"},
	{"x64/data/errorcodes/japanese.txt", "59f970b96d482fe0b34bff8a4edeb61d"},
	{"x64/data/errorcodes/korean.txt", "a27add49d85dfe94ca71c718e7387e25"},
	{"x64/data/errorcodes/mexican.txt", "a27add49d85dfe94ca71c718e7387e25"},
	{"x64/data/errorcodes/polish.txt", "a27add49d85dfe94ca71c718e7387e25"},
	{"x64/data/errorcodes/portuguese.txt", "59f970b96d482fe0b34bff8a4edeb61d"},
	{"x64/data/errorcodes/russian.txt", "a27add49d85dfe94ca71c718e7387e25"},
	{"x64/data/errorcodes/spanish.txt", "59f970b96d482fe0b34bff8a4edeb61d"},
	{"x64/metadata.dat", "ebf8ef82a7bc0fbbbf0560922333d99b"},
	{"update/update.rpf", "na"},
	{"update/x64/data/errorcodes/american.txt", "7ebf0ad90a2cdde27e0388ec70a2cdda"},
	{"update/x64/data/errorcodes/chinese.txt", "17757a647d006750be5a8d213bacf342"},
	{"update/x64/data/errorcodes/french.txt", "8b8746a40a84a0127bef0971b1a87e0f"},
	{"update/x64/data/errorcodes/german.txt", "5e59c88113a782d52cd2051995936775"},
	{"update/x64/data/errorcodes/italian.txt", "3fab47e12276c4a94eb8b2d9c2e46a25"},
	{"update/x64/data/errorcodes/japanese.txt", "c6a0e843104daa151fbca86aaf7b1753"},
	{"update/x64/data/errorcodes/korean.txt", "640d3929f2517459ace681105d85b15b"},
	{"update/x64/data/errorcodes/mexican.txt", "1969df3927ca6cdbd5d2c97bf3283148"},
	{"update/x64/data/errorcodes/polish.txt", "ff9c9bf17347582c4787982564e858f3"},
	{"update/x64/data/errorcodes/portuguese.txt", "844a538d54041c477f80a322cb702805"},
	{"update/x64/data/errorcodes/russian.txt", "8c26b266f576f2eed590e05076133005"},
	{"update/x64/data/errorcodes/spanish.txt", "979c9c64e9893bfbf1a523fe68485d19"},
	{"update/x64/dlcpacks/mpairraces/dlc.rpf", "ff7048ad6e21badb1e68f486a207091f"},
	{"update/x64/dlcpacks/mpapartment/dlc.rpf", "02cee302d599ef5af7940d1aa43cb921"},
	{"update/x64/dlcpacks/mpassault/dlc.rpf", "d7ba652435dafe28d41a9d362253834e"},
	{"update/x64/dlcpacks/mpbattle/dlc.rpf", "ac7ccdebad5d29d8d6697f46b50b87fc"},
	{"update/x64/dlcpacks/mpbattle/dlc1.rpf", "3c2e24b034cab10ee3e0ec75654420ed"},
	{"update/x64/dlcpacks/mpbiker/dlc.rpf", "cc2922c85f1a6667988dc37088169db2"},
	{"update/x64/dlcpacks/mpchristmas2/dlc.rpf", "a4d0910b796c06949ddecaf59e9e7abd"},
	{"update/x64/dlcpacks/mpchristmas2017/dlc.rpf", "d931ea0794c3380151b4d5496ec82b0c"},
	{"update/x64/dlcpacks/mpchristmas2018/dlc.rpf", "6b6d129c37e4e054c30880a8663a2137"},
	{"update/x64/dlcpacks/mpexecutive/dlc.rpf", "d37cf537a05358abe40f3256c7fd593f"},
	{"update/x64/dlcpacks/mpgunrunning/dlc.rpf", "787c0fdb8a419eadc04a2f35ba51b028"},
	{"update/x64/dlcpacks/mphalloween/dlc.rpf", "3ef847c2c5bbff72d09fdb8dd7a23367"},
	{"update/x64/dlcpacks/mpheist/dlc.rpf", "268ebd26d4e608e22c97a4406a441304"},
	{"update/x64/dlcpacks/mpimportexport/dlc.rpf", "4df92fb10454bf569b425acc223b4f1c"},
	{"update/x64/dlcpacks/mpjanuary2016/dlc.rpf", "ba7d62c1494faecf1f35254fa7a265e1"},
	{"update/x64/dlcpacks/mplowrider/dlc.rpf", "af18a05af6922b7af3daa353014cac21"},
	{"update/x64/dlcpacks/mplowrider2/dlc.rpf", "16aea3603b00ff6dc5fa53d1d27498d5"},
	{"update/x64/dlcpacks/mpluxe/dlc.rpf", "64656efa4b68075de5d36630eb3df1ce"},
	{"update/x64/dlcpacks/mpluxe2/dlc.rpf", "0b707b7b0c5fa846b0c7e3795bbb752d"},
	{"update/x64/dlcpacks/mppatchesng/dlc.rpf", "cbd02bff9d631941cb1be25323f577b9"},
	{"update/x64/dlcpacks/mpreplay/dlc.rpf", "2efea9ac2629bb3bdbe6bdc0e0d14894"},
	{"update/x64/dlcpacks/mpsmuggler/dlc.rpf", "de3154bbc19fa8e4d4f7c5ad8310cffd"},
	{"update/x64/dlcpacks/mpspecialraces/dlc.rpf", "924ba24cda50276419fd98a1e3d6b327"},
	{"update/x64/dlcpacks/mpstunt/dlc.rpf", "f44f2933345872808786bff1300386ee"},
	{"update/x64/dlcpacks/mpvalentines2/dlc.rpf", "572fbe80370d4c4e1afa3becbe6b2615"},
	{"update/x64/dlcpacks/mpxmas_604490/dlc.rpf", "0ca85b971ccae96b1342cdf62d0ef644"},
	{"update/x64/dlcpacks/patchday10ng/dlc.rpf", "1bcf10ae4fbdfc743339072b92d4a993"},
	{"update/x64/dlcpacks/patchday11ng/dlc.rpf", "b502c76cbf94653d8a9128b33714dd0c"},
	{"update/x64/dlcpacks/patchday12ng/dlc.rpf", "9b85eae38a859f4fec8ed289821fccf4"},
	{"update/x64/dlcpacks/patchday13ng/dlc.rpf", "ba4892e52e7b558aeab16716db1bb621"},
	{"update/x64/dlcpacks/patchday14ng/dlc.rpf", "d1bce738736bb767db87589c1362f8f5"},
	{"update/x64/dlcpacks/patchday15ng/dlc.rpf", "d48a49833f55b9db6727eb2d7a618b7c"},
	{"update/x64/dlcpacks/patchday16ng/dlc.rpf", "9e3387e2f33a1bfcc99bdeb059d6b919"},
	{"update/x64/dlcpacks/patchday17ng/dlc.rpf", "82a2d153b8e062fbd53779f0927a791d"},
	{"update/x64/dlcpacks/patchday18ng/dlc.rpf", "23185de5b5b262f92d4daa67c89f38ba"},
	{"update/x64/dlcpacks/patchday19ng/dlc.rpf", "05fb84f416cddee078443d18f753c1cc"},
	{"update/x64/dlcpacks/patchday1ng/dlc.rpf", "e0300b04dfae8d26335f8555ff817e71"},
	{"update/x64/dlcpacks/patchday20ng/dlc.rpf", "0e78879d7aa35c04a036e34d3eeda996"},
	{"update/x64/dlcpacks/patchday2bng/dlc.rpf", "944683e32536c908611a2f9bc2e0111c"},
	{"update/x64/dlcpacks/patchday2ng/dlc.rpf", "f48d5113ed030b737d84b323bfc7dc18"},
	{"update/x64/dlcpacks/patchday3ng/dlc.rpf", "8a53726d7a79d0dcb9019d1e479c2b8e"},
	{"update/x64/dlcpacks/patchday4ng/dlc.rpf", "67cf27f27c020f5196f1b96628e062dd"},
	{"update/x64/dlcpacks/patchday5ng/dlc.rpf", "ff98184310852ef5bc64ba9ae057086f"},
	{"update/x64/dlcpacks/patchday6ng/dlc.rpf", "46018995d24532336bec65173c37f1ef"},
	{"update/x64/dlcpacks/patchday7ng/dlc.rpf", "be057243bc5fa4dd6a68ac79a4db53c5"},
	{"update/x64/dlcpacks/patchday8ng/dlc.rpf", "991cc2b602e1f5dde41c258a83f7b9d0"},
	{"update/x64/dlcpacks/patchday9ng/dlc.rpf", "99813e1b9610fc940e8faedfcb16da86"},
	{"update/x64/metadata.dat", "aa5ebea98319ce3430da1a5989044e7e"}
};

GTAFilesChecker::GTAFilesChecker(QString base) : _base{std::move(base)}{

	_hasher = new MD5Hasher;

	connect(_hasher, &MD5Hasher::finished, this, &GTAFilesChecker::process);

	// SIGNALS REDIRECT
	connect(_hasher, &MD5Hasher::fileProcessing, this, &GTAFilesChecker::fileProcessing);
	connect(_hasher, &MD5Hasher::bytesProcessing, this, &GTAFilesChecker::bytesProcessing);

	_hasher->addFiles(s_hashes.keys(), _base);
}

GTAFilesChecker::~GTAFilesChecker(){
	_hasher->deleteLater();
}

bool GTAFilesChecker::rootRemoveAllUnofficialFiles(){
	QStringList files(QDir(_base).entryList(QStringList() << "*", QDir::Files | QDir::Hidden | QDir::NoSymLinks));
	for(QHash<QString, QString>::const_iterator it{s_hashes.constBegin()}; it != s_hashes.constEnd(); ++it){
		if(it.key().contains("/")) continue;
		files.removeOne(it.key());
	}
	bool ok = true;
	ok &= QDir(QString("%1/%2").arg(_base).arg("mods")).removeRecursively();
	ok &= QDir(QString("%1/%2").arg(_base).arg("scripts")).removeRecursively();
	ok &= QDir(QString("%1/%2").arg(_base).arg("disabledMods")).removeRecursively();
	ok &= deleteListRelative(files);
	return ok;
}

void GTAFilesChecker::check() const{
	_hasher->process();
}

void GTAFilesChecker::stop(){
	disconnect(_hasher, 0, 0, 0);
	_hasher->stop();
}

QStringList GTAFilesChecker::getErrors() const{
	return _md5Errors;
}

bool GTAFilesChecker::deleteCorrupted() const{
	return deleteListRelative(_md5Errors);
}

bool GTAFilesChecker::deleteListRelative(QStringList const& list) const{
	if(list.isEmpty()) return true;
	bool deleteOk = true;
	for(auto const& file : list){
		QFile f(QString("%1/%2").arg(_base).arg(file));
		if(!f.exists()) continue;
		f.setPermissions(QFile::ReadOther | QFile::WriteOther);
		deleteOk &= f.remove();
	}
	return deleteOk;
}

bool GTAFilesChecker::hasRealCorruptedFiles() const{
	return _existCount != 0;
}

void GTAFilesChecker::process(const QList<QPair<QString, QString>> &result){

	if(_hasher->isStopped()) return; // If it has been canceled, do not process
	_md5Errors.clear();
	_existCount = 0;

	bool isError = false;

	for(QList<QPair<QString, QString>>::const_iterator it = result.constBegin(); it != result.constEnd(); ++it){
		QString const relFile = it->first.mid(_base.length() + 1);
		QString hash{s_hashes[relFile]};
		if(hash != "na" && hash != it->second){
			_md5Errors << relFile;
			isError = true;
			if(!it->second.isEmpty()) _existCount++; // Could be a problem because we can have an empty file.
			qDebug() << relFile << "error md5 " << it->second << " should be " << s_hashes[relFile];
		}
	}

	if(isError)
		emit error();
	else
		emit success();
}
