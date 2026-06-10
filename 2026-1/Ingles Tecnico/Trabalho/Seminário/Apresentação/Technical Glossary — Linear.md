# Technical Glossary — Linear

> Source: [linear.app/docs](https://linear.app/docs) & [linear.app/developers](https://linear.app/developers)
> Terms are presented in their original English form with contextual explanations and practical examples.

---

## Part 1 — Technical Glossary
*Termos técnicos com trechos da documentação oficial (em inglês) e explicações contextuais*

---

### Issue

**Trecho da documentação oficial:**

> *"Issues are always linked to a single team. They have an issue ID (team's issue identifier and unique number) and are required to have a title and a status — all other properties and relations are optional."*
> — [linear.app/docs/creating-issues](https://linear.app/docs/creating-issues)

**Contexto técnico:**

Uma **Issue** é a unidade fundamental de trabalho no Linear. Cada issue recebe automaticamente um identificador único no formato `[SIGLA DO TEAM]-[NÚMERO]` (ex: `ENG-42`), que serve como referência universal dentro da plataforma e nas integrações com GitHub, Slack e outras ferramentas.

Além do ID automático, uma issue carrega propriedades opcionais como assignee (responsável), label (etiqueta), priority (prioridade), cycle (sprint) e project (projeto). Todas essas propriedades podem ser definidas na criação ou editadas depois — com exceção do time ao qual pertence, que é fixado no momento de criação.

**Exemplo prático:**

Uma issue `ENG-101` com título *"Fix login button on mobile"* é criada via atalho `C`, atribuída ao Dev. João, com prioridade `Urgent` e adicionada ao Cycle 12. Quando João abre um Pull Request no GitHub com a mensagem `Closes ENG-101`, a integração atualiza o status da issue automaticamente para *In Review*.

---

### Workflow / Issue Status

**Trecho da documentação oficial:**

> *"Issues statuses define the type and order of states that issues can move through from start to completion. These workflows are team-specific and come with a default set and order: Backlog > Todo > In Progress > Done > Canceled."*
> — [linear.app/docs/configuring-workflows](https://linear.app/docs/configuring-workflows)

> *"How we work at Linear — We have the following workflow set up for our product team: Backlog: Icebox, Backlog / Unstarted: Todo / Started: In Progress, In Review, Ready to Merge / Completed: Done / Canceled: Canceled, Could not reproduce, Won't Fix, Duplicate."*

**Contexto técnico:**

O **Workflow** no Linear é um conjunto de **status** organizados em cinco categorias fixas: *Triage*, *Backlog*, *Unstarted*, *Started*, *Completed* e *Canceled*. As categorias não podem ser reordenadas, mas os status dentro de cada categoria são completamente customizáveis — cada time pode criar, renomear, colorir e reorganizar seus status livremente.

Cada status pertence a uma categoria que define seu comportamento: issues na categoria *Started* são contabilizadas no velocity do time; issues na categoria *Completed* encerram ciclos de automação como o **Parent auto-close**.

**Exemplo prático:**

O time de design cria um status personalizado chamado *"Awaiting Feedback"* dentro da categoria *Started*. Isso permite que issues fiquem pausadas aguardando revisão sem sair do fluxo ativo, mantendo visibilidade no Cycle sem comprometer o progresso.

---

### Backlog

**Trecho da documentação oficial:**

> *"The default status defines the workflow status that will be applied to newly created issues in your team. By default, your first Backlog status will be the default status."*
> — [linear.app/docs/configuring-workflows](https://linear.app/docs/configuring-workflows)

**Contexto técnico:**

**Backlog** é a categoria de status que agrupa issues existentes mas ainda não priorizadas para um ciclo de trabalho. No Linear, funciona tanto como status padrão para issues recém-criadas quanto como área de triagem e planejamento.

O Backlog pode conter subestados personalizados — a própria Linear usa dois dentro dessa categoria: *Icebox* (ideias de longo prazo, sem prioridade imediata) e *Backlog* (itens a serem trabalhados em breve). Essa separação permite estratificar o volume de trabalho pendente sem poluir o fluxo ativo.

**Exemplo prático:**

Durante o Sprint Planning, o time revisa o Backlog e arrasta as issues mais urgentes para o Cycle atual. Issues que permanecem no Backlog por mais de 90 dias sem atualização podem ser arquivadas automaticamente via **Auto-archive**, configurável em `Settings → Teams → Auto-archive`.

---

### Cycle

**Trecho da documentação oficial:**

> *"Cycles are Linear's version of sprints — time-boxed periods during which a team focuses on a set of issues."*
> — [linear.app/docs/cycles](https://linear.app/docs/cycles)

**Contexto técnico:**

Um **Cycle** é um sprint com data de início e fim definidas, ao qual issues são associadas para indicar que serão trabalhadas naquele período. Diferente de ferramentas como Jira, o Linear não obriga o time a usar Cycles — é um recurso opcional que pode ser ativado por time individualmente.

Ao final de um Cycle, issues incompletas podem ser transferidas (*rolled over*) automaticamente para o próximo, mantendo rastreabilidade. O Linear também gera métricas automáticas de cada Cycle: issues completadas, velocity, taxa de conclusão e burn-down.

**Exemplo prático:**

O Cycle 24 começa em 13/06 com 12 issues comprometidas. No último dia, 9 foram concluídas. As 3 restantes são transferidas automaticamente para o Cycle 25 com uma anotação de que foram *rolled over*, preservando o histórico de planejamento.

---

### Project

**Trecho da documentação oficial:**

> *"Projects are units of work that have a clear outcome or planned completion date, such as a new feature's launch, and are comprised of issues and optional documents. They can be shared across multiple teams and come with their own unique features, progress graph, and notification options."*
> — [linear.app/docs/projects](https://linear.app/docs/projects)

> *"The project lead is in charge of writing the spec and general execution. Other team members collaborate on the brief, split up areas of work, and then write their own issues."*

**Contexto técnico:**

Um **Project** agrupa issues relacionadas a uma entrega específica — como o lançamento de uma feature, uma migração técnica ou uma campanha. Ao contrário de Cycles (que são temporais), Projects são orientados a *outcome*: existem até a entrega ser concluída, independentemente de quantos Cycles passaram.

Projects possuem um **progress graph** automático, calculado a partir do status das issues associadas. Também suportam **milestones** (marcos intermediários), documentos internos, e podem cruzar múltiplos times — uma issue do time de backend e outra do frontend podem coexistir no mesmo Project.

**Exemplo prático:**

O Project *"Checkout Redesign"* tem 20 issues distribuídas entre os times de Design (`DES`), Frontend (`WEB`) e Backend (`API`). O Project Lead, Maria, acompanha o progresso pelo gráfico automático e registra uma atualização semanal de status — *On Track*, *At Risk* ou *Off Track* — visível para toda a organização.

---

### Sub-issue

**Trecho da documentação oficial:**

> *"Consider creating sub-issues when a set of work is too large to be a single issue but too small to be a project. Sub-issues are also ideal for splitting up work shared across teammates. When you add a sub-issue to another issue, the other issue becomes its 'parent'."*
> — [linear.app/docs/parent-and-sub-issues](https://linear.app/docs/parent-and-sub-issues)

> *"Sometimes an issue grows so large it's more appropriate to turn it into a project instead. To do so, hover over the parent's ... menu and choose 'Convert to project.' The project will inherit its details from the original parent issue, and former sub-issues will become standard issues in the project."*

**Contexto técnico:**

**Sub-issues** estabelecem uma hierarquia dentro do sistema de issues: uma *parent issue* contém múltiplas *sub-issues*, cada uma rastreável individualmente com seu próprio status, assignee e prioridade. Quando todas as sub-issues são concluídas, a parent pode ser fechada automaticamente via **Parent auto-close** (configurável em `Settings → Team → Workflow`).

É possível converter uma issue em sub-issue de outra, ou promover uma parent issue a Project quando o escopo cresce — nesse caso, as sub-issues tornam-se issues regulares do projeto.

**Exemplo prático:**

A issue `ENG-55` *"Implement authentication module"* é grande demais para uma única tarefa. O dev a divide em sub-issues: `ENG-56` (OAuth integration), `ENG-57` (session management) e `ENG-58` (tests). Quando as três são marcadas como *Done*, a `ENG-55` é fechada automaticamente.

---

### Triage

**Trecho da documentação oficial:**

> *"Triage is an additional status category that acts as an Inbox for your team. Triage is particularly powerful when combined with other integrations like Asks, Slack, or our support ticketing integrations."*
> — [linear.app/docs/configuring-workflows](https://linear.app/docs/configuring-workflows)

**Contexto técnico:**

**Triage** é uma categoria especial de status que funciona como ponto de entrada (*intake*) para issues geradas por canais externos — Slack, e-mail, integrações de suporte — antes de serem avaliadas e inseridas no fluxo de trabalho normal. A diferença em relação ao Backlog é intencional: issues no Triage ainda não foram qualificadas, enquanto issues no Backlog já foram avaliadas e aceitas pelo time.

O Triage é ativado por time em `Settings → Teams → Triage` e pode ser combinado com o recurso **Linear Asks** para gerenciar solicitações internas vindas de outros times da organização.

**Exemplo prático:**

Um cliente reporta um bug via Intercom. A integração cria automaticamente uma issue no Triage do time de engenharia. Um dev de plantão avalia: se for um bug real, move para Backlog com prioridade; se for dúvida de usuário, cancela com nota explicativa. Nenhuma issue passa direto para o fluxo sem avaliação humana.

---

### API (GraphQL)

**Trecho da documentação oficial:**

> *"Linear's public API is built using GraphQL. It's the same API we use internally for developing our applications."*
> — [linear.app/developers/graphql](https://linear.app/developers/graphql)

> *"Issues can be created using our GraphQL API. Many integrations in our directory allow creating issues from other services using this functionality."*
> — [linear.app/docs/creating-issues](https://linear.app/docs/creating-issues)

**Contexto técnico:**

A **API GraphQL** do Linear permite que desenvolvedores acessem e manipulem programaticamente todos os recursos da plataforma: criar issues, atualizar status, consultar cycles, gerenciar membros de time, entre outros. Por ser GraphQL, cada requisição retorna exatamente os campos solicitados — sem over-fetching ou under-fetching de dados.

A autenticação é feita via **OAuth 2.0** (para aplicações de terceiros) ou **Personal API Key** (para automações pessoais e integrações diretas). O endpoint único é `https://api.linear.app/graphql`.

**Exemplo prático:**

Um script de automação consulta diariamente todas as issues com prioridade `Urgent` abertas há mais de 72 horas e posta um resumo no canal `#engineering` do Slack. Isso é feito via uma query GraphQL que filtra por `priority: urgent` e `createdAt_lt: [timestamp]`, sem depender de nenhuma interface manual.

---

### Webhook

**Trecho da documentação oficial:**

> *"Webhooks allow you to receive HTTP POST requests to a URL of your choice when events occur in your Linear workspace."*
> — [linear.app/developers/webhooks](https://linear.app/developers/webhooks)

**Contexto técnico:**

**Webhooks** são notificações HTTP disparadas automaticamente pelo Linear quando eventos específicos ocorrem — como criação de issue, mudança de status, atribuição de assignee ou fechamento de um Cycle. Diferente da API (que exige que o cliente consulte ativamente), webhooks enviam os dados para um endpoint externo no momento em que o evento acontece (*event-driven*).

São configurados em `Settings → API → Webhooks`, com suporte a filtragem por tipo de recurso (Issues, Projects, Cycles etc.) e por time específico.

**Exemplo prático:**

Um webhook escuta eventos do tipo `Issue.update` no time `ENG`. Toda vez que uma issue muda para o status *Done*, o servidor externo recebe o payload JSON com os dados da issue e registra o evento no sistema interno de métricas de produtividade da empresa.

---

### Initiative

**Trecho da documentação oficial:**

> *"Initiatives allow you to organize projects under a strategic goal."*
> — [linear.app/docs/initiatives](https://linear.app/docs/initiatives)

**Contexto técnico:**

**Initiatives** representam o nível mais alto da hierarquia do Linear: `Initiative → Projects → Issues → Sub-issues`. Uma Initiative agrupa múltiplos Projects que contribuem para um mesmo objetivo estratégico de médio ou longo prazo — como *"Expand to enterprise market"* ou *"Improve platform performance by 50%"*.

O progresso de uma Initiative é calculado automaticamente com base no status de seus Projects associados, oferecendo visibilidade de alto nível para liderança e stakeholders sem precisar acessar cada issue individualmente.

**Exemplo prático:**

A Initiative *"Product Launch v3.0"* agrupa quatro Projects: *New Onboarding Flow*, *API v3*, *Mobile App Redesign* e *Documentation Update*. O VP de Produto acompanha o progresso geral pela Initiative — sem precisar entrar em cada Project — e publica atualizações de status mensais para o board da empresa.

---

## Part 2 — Glossary
*Minimum 10 technical terms with short definitions in English*

---

| Term | Definition |
|------|------------|
| **Issue** | The basic unit of work in Linear. Each issue has a unique ID, a required title and status, and optional properties like assignee, priority, label, cycle, and project. |
| **Workflow** | A team-specific set of statuses that define the states an issue can move through, from creation to completion. Default order: Backlog → Todo → In Progress → Done → Canceled. |
| **Backlog** | A workflow status category for issues that exist but have not yet been assigned to a cycle. The default status for newly created issues. |
| **Cycle** | A time-boxed sprint during which a team commits to completing a defined set of issues. Unfinished issues can be rolled over to the next cycle automatically. |
| **Project** | A unit of work oriented toward a specific outcome or delivery date, grouping related issues and documents. Can span multiple teams and includes a built-in progress graph. |
| **Sub-issue** | A child issue nested under a parent issue, used to break down large tasks into smaller, individually trackable pieces of work. |
| **Parent issue** | An issue that contains one or more sub-issues. Its completion can be automated when all sub-issues are marked as done via the *Parent auto-close* setting. |
| **Triage** | A special intake status category that acts as an inbox for issues arriving from external sources (Slack, email, integrations) before they are reviewed and moved into the regular workflow. |
| **Initiative** | The highest level of work organization in Linear. Groups multiple projects under a single strategic goal and provides aggregate progress tracking across teams. |
| **API (GraphQL)** | Linear's public programmatic interface, built with GraphQL. Allows developers to create, read, and update all workspace data via the endpoint `https://api.linear.app/graphql`. |
| **Webhook** | An event-driven HTTP notification sent by Linear to an external URL when a specific event occurs in the workspace, such as an issue status change or a new comment. |
| **Milestone** | A named checkpoint within a project used to mark significant intermediate deliverables. Milestones help track progress toward the final project outcome. |
| **Label** | A tag applied to an issue to categorize it by type, area, or any custom classification (e.g., `bug`, `feature`, `design`). Labels can be defined at workspace or team level. |
| **Assignee** | The team member responsible for completing a given issue. Issues can have a single assignee plus additional members listed as collaborators. |
| **Priority** | A property that defines the urgency of an issue. Linear uses four levels: `Urgent`, `High`, `Medium`, and `Low`, plus `No priority`. |
| **OAuth 2.0** | The authentication protocol used by Linear's API to allow third-party applications to act on behalf of a user without exposing their credentials. Required for building public Linear integrations. |

---

*Source: [linear.app/docs](https://linear.app/docs) — Linear Official Documentation*
*Source: [linear.app/developers](https://linear.app/developers) — Linear Developer Documentation*