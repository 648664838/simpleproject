//���Ӵ���
enum emConError
{
	EMCONERROR_WIN_SOCKET_INIT = -100, //winsocket������ʼ��ʧ��	
	EMCONERROR_HASCONNECT = -101, //������
	EMCONERROR_HAS_LOAD_CONINFO = -102, //�Ѽ���
	EMCONERROR_SERVER_NOT_OPEN = -103, //δ���ļ�
	EMCONERROR_SERVERFILE_IS_NULL = -104, //�ļ�����Ϊ��
	EMCONERROR_SERVER_INDEX_IS_NULL = -105, //���Ե�����Ϊ��
	EMCONERROR_SERVER_DATA_IS_NULL = -106, //��ѡ�еķ���������ΪNULL
	EMCONERROR_SERVER_DATA_NOT_FULL = -107, //��ѡ�еķ��������ݲ�ȫ
	EMCONERROR_SOCKET_CREATE = -108, //������socket����
	EMCONERROR_NET_NOT_CON = -109, //����δ����
	EMCONERROR_CONTEXT_IS_NULL = -110, //������Ϣ���ݵĻ���Ϊ��
	EMCONERROR_CONNECT_USER_MAX = -111, //���ӵ��û������ﵽ���
};

enum emMessageQueueError
{
	ERROR_PUSH_MESSAGE_QUEUE_FULL = -200, //pushʱ����������
	ERROR_POP_MESSAGE_QUEUE_FULL = -201, //popʱ������Ϊ��
	ERROR_SELECT_MESSAGE_HADNLE = -202,//select������Ϣʱ�򣬳���
	ERROR_MESSAGE_USE_INDEX = -203, //��Ϣ���û������Ƿ�
};

enum emLoginSceneError
{
	ERROR_LOGIN_SCENE_NOT_CONNECT    = -300, //δ���ӻ��ѵ�½
	ERROR_LOGIN_SCENE_ACOUNT_INVAILD = -301, //�˺Ż��������
	ERROR_LOGIN_SCENE_ROLEID_ZERO    = -302, //���ص�RoleIDΪ0
	ERROR_LOGIN_SCENE_ROLEDATA_NULL	 = -303, //���ص�Role����ΪNULL
	ERROR_LOGIN_SCENE_ROLEDATA_RESULT  = -304, //���ص�Role���ݽ������
	ERROR_LOGIN_SCENE_ROLEDATA_INVALLD = -305, //���ص�Role���ݲ��Գ���
	ERROR_LOGIN_SCENE_ROLEDATA_FECTH   = -306, //���ص�Role���ݳ���
};

enum emCreateAccountError
{
	ERROR_CREATE_ACCOUNT_EXIST		 = -400, //�˺��Ѵ���
	ERROR_CREATE_ACCOUNT_ACCOUNT	 = -401, //�����˺ų���
	ERROR_CREATE_ACCOUNT_ROLE		 = -402, //����������ݳ���

};